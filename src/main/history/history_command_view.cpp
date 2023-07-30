#include <main/history/history_command_view.h>
#include <zaf/object/type_definition.h>
#include "main/preview_control_creating.h"

namespace ra {

ZAF_DEFINE_TYPE(PreservedCommandView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/history/history_command_view.xaml")
ZAF_DEFINE_TYPE_END;

PreservedCommandView::PreservedCommandView(
    CommandInputContent command_input_content,
    std::unique_ptr<mod::Command> command)
    : 
    command_input_content_(std::move(command_input_content)),
    command_(std::move(command)) {

    ZAF_EXPECT(command_);
}


void PreservedCommandView::AfterParse() {

    __super::AfterParse();

    commandEdit->SetStyle(CommandDisplayStyle::Preserved);
    commandEdit->SetInputContent(command_input_content_);

    auto preview_control = CreateCommandPreviewControl(*command_);
    preview_control->SetStyle(CommandDisplayStyle::Preserved);
    preview_control->SetIsVisible(true);

    previewContainer->AddChild(preview_control);

    Subscriptions() += MouseEnterEvent().Subscribe(std::bind([this]() {
        toolBar->SetIsVisible(true);
    }));

    Subscriptions() += MouseLeaveEvent().Subscribe(std::bind([this]() {
        if (!this->ContainMouse()) {
            toolBar->SetIsVisible(false);
        }
    }));

    Subscriptions() += closeButton->ClickEvent().Subscribe(std::bind([this]() {
        close_event_.Raise(zaf::As<PreservedCommandView>(shared_from_this()));
    }));
}

}