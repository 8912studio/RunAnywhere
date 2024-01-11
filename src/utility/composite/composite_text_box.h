#pragma once

#include <zaf/control/control.h>
#include <zaf/window/popup_menu.h>
#include "utility/composite/composite_control.h"

namespace ra::utility::composite {

class CompositeTextBox : public zaf::Control, public CompositeControl {
public:
    ZAF_DECLARE_TYPE;

protected:
    void Initialize() override;

    void OnMouseCursorChanging(const zaf::MouseCursorChangingInfo&) override;
    void OnMouseDown(const zaf::MouseDownInfo&) override;
    void OnMouseUp(const zaf::MouseUpInfo&) override;
    void OnPreMouseMove(const zaf::PreMouseMoveInfo&) override;
    void OnDoubleClick(const zaf::DoubleClickInfo&) override;

    void OnKeyDown(const zaf::KeyDownInfo&) override;

    void OnFocusGained(const zaf::FocusGainedInfo&) override;
    void OnFocusLost(const zaf::FocusLostInfo&) override;

private:
    void HandleLeftButtonDown(const zaf::MouseDownInfo&);
    void HandleRightButtonDown(const zaf::MouseDownInfo& event_info);
    void CopySelectionToClipboard();

private:
    std::optional<zaf::Point> begin_selection_position_;
    std::weak_ptr<zaf::PopupMenu> popup_menu_;
};

}