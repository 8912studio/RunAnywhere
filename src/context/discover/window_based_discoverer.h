#pragma once

#include "context/discover/discoverer.h"

namespace ra::context {

class WindowBasedDiscoverer : public Discoverer {
public:
    //Make public for unittest.
    static ActivePath DecodeActivePath(const std::wstring& encoded);

public:
    ~WindowBasedDiscoverer();

    ActivePath Discover(const ForegroundWindowInfo& foreground_window_info) override;

private:
    static LRESULT CALLBACK ClientWindowProcedure(HWND, UINT, WPARAM, LPARAM);

private:
    void TryToInitialize();
    void TryToRegisterClientWindowClass();
    void TryToCreateClientWindow();

    ActivePath DiscoverActivePath(DWORD foreground_process_id);
    void ReceiveCopyDataMessage(const COPYDATASTRUCT& copy_data_info);

private:
    ATOM client_window_class_atom_{};
    HWND client_window_handle_{};

    std::size_t sequence_seed_{};
    std::size_t current_sequence_{};
    std::wstring response_buffer_;
};

}