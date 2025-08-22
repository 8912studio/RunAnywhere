#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include <objbase.h>
#include <zaf/base/error/com_error.h>
#include <zaf/rx/scheduler/main_thread_scheduler.h>
#include <zaf/rx/scheduler/single_thread_scheduler.h>
#include "context/discover/composite_discoverer.h"
#include "context/discover/everything_discoverer.h"
#include "context/discover/explorer_discoverer.h"
#include "context/discover/vscode_discoverer.h"
#include "context/discover/window_based_discoverer.h"
#include "utility/log/log.h"

namespace ra::context {
namespace {

std::shared_ptr<zaf::rx::Scheduler> GetDiscoverScheduler() {

    static auto discover_scheduler = []() {

        auto scheduler = std::make_shared<zaf::rx::SingleThreadScheduler>();

        //Make the new thread initialize COM first.
        scheduler->ScheduleWork([]() {
            HRESULT result = CoInitializeEx(0, COINIT_MULTITHREADED);
            ZAF_THROW_IF_COM_ERROR(result);
        });

        return scheduler;
    }();

    return discover_scheduler;
}


Discoverer& GetSyncDiscoverer() {
    static CompositeDiscoverer discoverer{
        []() {
            std::vector<std::unique_ptr<Discoverer>> discoverers;
            discoverers.push_back(std::make_unique<VSCodeDiscoverer>());
            return discoverers;
        }()
    };
    return discoverer;
}


Discoverer& GetAsyncDiscoverer() {
    static CompositeDiscoverer discoverer{
        []() {
            std::vector<std::unique_ptr<Discoverer>> discoverers;
            discoverers.push_back(std::make_unique<ExplorerDiscoverer>());
            discoverers.push_back(std::make_unique<EverythingDiscoverer>());
            discoverers.push_back(std::make_unique<WindowBasedDiscoverer>());
            return discoverers;
        }()
    };
    return discoverer;
}

}


zaf::rx::Observable<DesktopContext> DiscoverDesktopContext() {

    // We must retrieve foreground window info on main thread, rather than retrieving it on the
    // scheduler thread, as the main window might become the foreground window before the 
    // retrieving.
    ForegroundWindowInfo foreground_window_info;
    foreground_window_info.window_handle = GetForegroundWindow();
    if (!foreground_window_info.window_handle) {
        return zaf::rx::Observable<DesktopContext>::Just({});
    }

    GetWindowThreadProcessId(
        foreground_window_info.window_handle,
        &foreground_window_info.process_id);

    RA_LOG() << "Foreground window handle: " << foreground_window_info.window_handle << ", "
        << "process id: " << foreground_window_info.process_id;

    // Some discovers must execute on the main thread, before the main window becomes the 
    // foreground window.
    auto active_path = GetSyncDiscoverer().Discover(foreground_window_info);
    if (active_path) {

        // Return the result async to keep consistent with the async discoverer.
        return zaf::rx::Observable<DesktopContext>::Just(DesktopContext{ *active_path })
            .ObserveOn(zaf::rx::MainThreadScheduler::Instance());
    }

    // Some discovers do not need to execute on the main thread, so we execute them on a separate
    // scheduler thread to avoid hanging, which may slow down the popup speed of the main window.
    return zaf::rx::Observable<DesktopContext>::CreateOn(
        GetDiscoverScheduler(), 
        [foreground_window_info](zaf::rx::Subscriber<DesktopContext> subscriber) {

            DesktopContext result;
            auto active_path = GetAsyncDiscoverer().Discover(foreground_window_info);
            if (active_path) {
                result.active_path = *active_path;
            }
            subscriber.OnNext(result);
            subscriber.OnCompleted();
        }
    ).ObserveOn(zaf::rx::MainThreadScheduler::Instance());
}

}