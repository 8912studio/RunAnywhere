#include "utility/composite/composite_control.h"
#include <zaf/base/as.h>
#include <zaf/control/control.h>

namespace ra::utility::composite {

bool CompositeControl::IsPositionInsideTextBoundary(const zaf::Point& mouse_position) {

    bool result{};
    VisitComposableControls(mouse_position, [&result](VisitContext& context) {
    
        const auto& translated_position = context.TranslatedPosition();
        if (context.Control()->RectInSelf().Contain(translated_position)) {
            result = context.Composable()->IsPositionInsideTextBoundary(translated_position);
            context.MarkAsAborted();
        }
    });
    return result;
}


void CompositeControl::BeginSelection(const zaf::Point& position) {

    VisitComposableControls(position, [](VisitContext& context) {
        context.Composable()->BeginSelection(context.TranslatedPosition());
    });
}


void CompositeControl::ChangeSelection(const PositionRange& position_range) {

    VisitComposableControls(position_range, [](VisitContext& context) {
        context.Composable()->ChangeSelection(context.TranslatedPositionRange());
    });
}


void CompositeControl::EndSelection() {

    VisitComposableControls({}, [](VisitContext& context) {
        context.Composable()->EndSelection();
    });
}


void CompositeControl::SelectWord(const zaf::Point& position) {

    VisitComposableControls(position, [](VisitContext& context) {

        const auto& translated_position = context.TranslatedPosition();
        if (context.Control()->RectInSelf().Contain(translated_position)) {
            context.Composable()->SelectWord(translated_position);
            context.MarkAsAborted();
        }
    });
}


void CompositeControl::BuildSelectedText(SelectedTextBuilder& builder) {

    VisitComposableControls({}, [&builder](VisitContext& context) {

        auto region_guard = builder.BeginRegion();
        context.Composable()->BuildSelectedText(builder);
    });
}


void CompositeControl::ChangeFocus(bool is_focused) {

    VisitComposableControls({}, [is_focused](VisitContext& context) {
        context.Composable()->ChangeFocus(is_focused);
    });
}


void CompositeControl::VisitComposableControls(
    TranslatedData translated_data, 
    const ComposableVisitor& visitor) {

    auto& control = zaf::As<zaf::Control>(*this);
    VisitContext context{ &control, std::move(translated_data) };
    VisitControl(context, visitor);
}


void CompositeControl::VisitControl(
    VisitContext& context,
    const ComposableVisitor& visitor) {

    for (const auto& each_child : context.Control()->Children()) {

        auto child_context = context.ToChild(*each_child);

        auto composable = child_context.Composable();
        if (composable) {
            visitor(child_context);
        }
        else {
            VisitControl(child_context, visitor);
        }

        if (child_context.IsAborted()) {
            context.MarkAsAborted();
            break;
        }
    }
}


CompositeControl::VisitContext CompositeControl::VisitContext::ToChild(zaf::Control& child) const {

    auto child_translated_data = std::visit([this, &child](const auto& value) {

        using DataType = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<DataType, zaf::Point>) {
            return TranslatedData{ control_->TranslatePositionToChild(value, child) };
        }
        else if constexpr (std::is_same_v<DataType, PositionRange>) {
            return TranslatedData{
                PositionRange{
                    control_->TranslatePositionToChild(value.Begin(), child),
                    control_->TranslatePositionToChild(value.End(), child)
                }
            };
        }
        return TranslatedData{};
    },
    translated_data_);

    return VisitContext{ &child, std::move(child_translated_data) };
}

}
