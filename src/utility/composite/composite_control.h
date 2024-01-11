#pragma once

#include <variant>
#include <zaf/control/control.h>
#include "utility/composite/composable_control.h"

namespace ra::utility::composite {

class CompositeControl : public ComposableControl {
public:
    bool IsPositionInsideTextBoundary(const zaf::Point& mouse_position) override;

    void BeginSelection(const zaf::Point& position) override;
    void ChangeSelection(const PositionRange& position_range) override;
    void EndSelection() override;

    void SelectWord(const zaf::Point& position) override;
    void BuildSelectedText(SelectedTextBuilder& builder) override;

    void ChangeFocus(bool is_focused) override;

private:
    using TranslatedData = std::variant<zaf::Point, PositionRange>;

    class VisitContext {
    public:
        VisitContext(zaf::Control* control, TranslatedData translated_data) :
            control_(control),
            translated_data_(std::move(translated_data)) {

        }

        zaf::Control* Control() const {
            return control_;
        }

        ComposableControl* Composable() const {
            return dynamic_cast<ComposableControl*>(control_);
        }

        const zaf::Point& TranslatedPosition() const {
            return std::get<zaf::Point>(translated_data_);
        }

        const PositionRange& TranslatedPositionRange() const {
            return std::get<PositionRange>(translated_data_);
        }

        bool IsAborted() const {
            return is_aborted_;
        }

        void MarkAsAborted() {
            is_aborted_ = true;
        }

        VisitContext ToChild(zaf::Control& control) const;

    private:
        zaf::Control* control_{};
        TranslatedData translated_data_;
        bool is_aborted_{};
    };

    using ComposableVisitor = std::function<void(VisitContext&)>;

private:
    void VisitComposableControls(
        TranslatedData translated_data,
        const ComposableVisitor& visitor);

    static void VisitControl(
        VisitContext& context,
        const ComposableVisitor& visitor);
};

}