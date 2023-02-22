#include "utility/thin_border_window.h"
#include <dwmapi.h>
#include <zaf/graphic/dpi.h>

namespace ra::utility {

void ThinBorderWindow::Initialize() {

    __super::Initialize();

    SetIsSizable(false);
    SetCanMaximize(false);
    SetCanMinimize(false);
    SetIsToolWindow(true);
    SetHasTitleBar(false);
    SetHasSystemMenu(false);
}


void ThinBorderWindow::OnHandleCreated(const zaf::HandleCreatedInfo& event_info) {

    __super::OnHandleCreated(event_info);

    //We need to extend non client area right after window handle is created, for those windows 
    //that will never be activated.
    ExtendNonClientArea(false);
}


void ThinBorderWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    if (event_info.Message().ID() == WM_NCCALCSIZE) {
        HandleCalculateNonClientSizeMessage(event_info.Message());
        event_info.MarkAsHandled(0);
    }
    else if (event_info.Message().ID() == WM_ACTIVATE) {
        HandleActivateMessage(zaf::ActivateMessage{ event_info.Message() });
    }

    __super::OnMessageReceived(event_info);
}


void ThinBorderWindow::HandleCalculateNonClientSizeMessage(const zaf::Message& message) {

    //Originally, we set all borders to 1 point thickness to remove the title bar, but it was not
    //work on Windows 10. So we use this work around: set top border to 0 to remove it completely,
    //and call DwmExtendFrameIntoClientArea() to extend a little non client area into top border.

    auto nc_border_in_pixels = static_cast<int>(zaf::FromDIPs(1, this->GetDPI()));

    RECT* adjusted_rect{};
    if (message.WParam() == TRUE) {
        adjusted_rect = &(reinterpret_cast<NCCALCSIZE_PARAMS*>(message.LParam())->rgrc[0]);
    }
    else {
        adjusted_rect = reinterpret_cast<RECT*>(message.LParam());
    }

    adjusted_rect->left += nc_border_in_pixels;
    adjusted_rect->right -= nc_border_in_pixels;
    adjusted_rect->bottom -= nc_border_in_pixels;
}


void ThinBorderWindow::HandleActivateMessage(const zaf::ActivateMessage& message) {

    ExtendNonClientArea(message.ActivateState() != zaf::ActivateState::Inactive);
}


void ThinBorderWindow::ExtendNonClientArea(bool is_active) {

    //Work around for Windows 10: 1 point thickness is not enough if the window is inactive, so we
    //use 2 point thickness.
    float extended_thickness = is_active ? 1.f : 2.f;

    MARGINS extended_margins{};
    extended_margins.cyTopHeight =
        static_cast<int>(zaf::FromDIPs(extended_thickness, this->GetDPI()));

    DwmExtendFrameIntoClientArea(this->Handle(), &extended_margins);
}


void ThinBorderWindow::OnMessageHandled(const zaf::MessageHandledInfo& event_info) {

    __super::OnMessageHandled(event_info);

    if (event_info.Message().ID() == WM_PAINT) {

        RECT paint_rect{};
        GetClientRect(this->Handle(), &paint_rect);
        paint_rect.bottom = static_cast<LONG>(zaf::FromDIPs(1, this->GetDPI()));

        HDC dc = GetDC(this->Handle());
        FillRect(dc, &paint_rect, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        ReleaseDC(this->Handle(), dc);
    }
}

}