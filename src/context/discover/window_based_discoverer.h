#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class WindowBasedDiscoverer : public Discoverer {
public:
    //TODO: Move this function out to a common place.
    static ActivePath DecodeActivePath(const std::wstring& encoded);

public:
    ~WindowBasedDiscoverer();

    std::optional<ActivePath> Discover(
        const ForegroundWindowInfo& foreground_window_info) override;

private:
    static LRESULT CALLBACK ClientWindowProcedure(HWND, UINT, WPARAM, LPARAM);

private:
    bool TryToInitialize();
    void TryToRegisterClientWindowClass();
    void TryToCreateClientWindow();

    std::optional<ActivePath> DiscoverActivePath(DWORD foreground_process_id);
    void ReceiveCopyDataMessage(const COPYDATASTRUCT& copy_data_info);

private:
    ATOM client_window_class_atom_{};
    HWND client_window_handle_{};

    std::size_t sequence_seed_{};
    std::size_t current_sequence_{};
    std::wstring response_buffer_;
};

}