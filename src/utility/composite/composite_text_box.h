#pragma once

#include <zaf/base/event/event.h>
#include <zaf/control/control.h>
#include <zaf/window/popup_menu.h>
#include "utility/composite/composite_control.h"
#include "utility/composite/position_range.h"

namespace ra::utility::composite {

class CompositeTextBoxSelectionChangedInfo;

class CompositeTextBox : public zaf::Control, public CompositeControl {
public:
    ZAF_OBJECT;

    zaf::Observable<CompositeTextBoxSelectionChangedInfo> SelectionChangedEvent() const {
        return selection_changed_event_.GetObservable();
    }

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
    void InnerChangeSelection(const PositionRange& position_range);
    void CopySelectionToClipboard();

private:
    std::optional<zaf::Point> begin_selection_position_;
    zaf::Event<CompositeTextBoxSelectionChangedInfo> selection_changed_event_;
    std::weak_ptr<zaf::PopupMenu> popup_menu_;
};

ZAF_OBJECT_BEGIN(CompositeTextBox)
ZAF_OBJECT_END;


class CompositeTextBoxSelectionChangedInfo {
public:
    explicit CompositeTextBoxSelectionChangedInfo(const PositionRange& position_range) : 
        position_range_(position_range) {

    }

    const composite::PositionRange& PositionRange() const {
        return position_range_;
    }

private:
    composite::PositionRange position_range_;
};

}