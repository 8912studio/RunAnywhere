#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include <zaf/base/error/com_error.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include "context/discover/composite_discoverer.h"

namespace ra::context {
namespace {

std::shared_ptr<zaf::Scheduler> GetDiscoverScheduler() {

    static auto discover_scheduler = []() {

        auto scheduler = zaf::Scheduler::CreateOnSingleThread();

        //Make the new thread initialize COM first.
        scheduler->Schedule([]() {
            HRESULT result = CoInitializeEx(0, COINIT_MULTITHREADED);
            ZAF_THROW_IF_COM_ERROR(result);
        });

        return scheduler;
    }();

    return discover_scheduler;
}


ActivePath DiscoverActivePath(const ForegroundWindowInfo& foreground_window_info) {

    static CompositeDiscoverer discoverer;
    auto result = discoverer.Discover(foreground_window_info);
    if (result) {
        return *result;
    }
    return ActivePath{};
}

}


zaf::Observable<DesktopContext> DiscoverDesktopContext() {

    //We must get foreground window info on main thread, rather than getting it on scheduler 
    //thread, due to main window might be set foreground before getting.
    ForegroundWindowInfo foreground_window_info;
    foreground_window_info.window_handle = GetForegroundWindow();
    if (!foreground_window_info.window_handle) {
        return zaf::rx::Just(DesktopContext{});
    }

    GetWindowThreadProcessId(
        foreground_window_info.window_handle,
        &foreground_window_info.process_id);

    //Discovering could be slow, so we do it on a separate scheduler thread to avoid hanging.
    return zaf::rx::Create<DesktopContext>(
        GetDiscoverScheduler(), 
        [foreground_window_info](zaf::Observer<DesktopContext> observer) {

            DesktopContext result;
            result.active_path = DiscoverActivePath(foreground_window_info);
            observer.OnNext(result);
        }
    ).ObserveOn(zaf::Scheduler::Main());
}

}