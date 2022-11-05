#include "context/desktop_context_discovering.h"
#include <Windows.h>
#include <zaf/base/error/com_error.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include "context/discover/composite_discoverer.h"

namespace ra::context {
namespace {

ActivePath DiscoverActivePath(const ForegroundWindowInfo& foreground_window_info) {

    static CompositeDiscoverer discoverer;
    auto result = discoverer.Discover(foreground_window_info);
    if (result) {
        return *result;
    }
    return ActivePath{};
}


DesktopContext InnerDiscoverDesktopContext() {

    ForegroundWindowInfo foreground_window_info;
    foreground_window_info.window_handle = GetForegroundWindow();
    if (!foreground_window_info.window_handle) {
        return {};
    }

    GetWindowThreadProcessId(
        foreground_window_info.window_handle,
        &foreground_window_info.process_id);

    DesktopContext result;
    result.active_path = DiscoverActivePath(foreground_window_info);
    return result;
}

}


zaf::Observable<DesktopContext> DiscoverDesktopContext() {

    static auto discover_scheduler = []() {

        auto scheduler = zaf::Scheduler::CreateOnSingleThread();
        scheduler->Schedule([]() {

            HRESULT result = CoInitializeEx(0, COINIT_MULTITHREADED);
            ZAF_THROW_IF_COM_ERROR(result);
        });

        return scheduler;
    }();

    return zaf::rx::Create<DesktopContext>(
        discover_scheduler, 
        [](zaf::Observer<DesktopContext> observer) {

        observer.OnNext(InnerDiscoverDesktopContext());
    })
    .ObserveOn(zaf::Scheduler::Main());
}

}