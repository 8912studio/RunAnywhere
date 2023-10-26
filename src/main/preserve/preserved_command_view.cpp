#include "main/preserve/preserved_command_view.h"
#include <zaf/object/type_definition.h>
#include "main/preview_control_creating.h"

namespace ra {

ZAF_DEFINE_TYPE(PreservedCommandView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/preserve/preserved_command_view.xaml")
ZAF_DEFINE_TYPE_END;

PreservedCommandView::PreservedCommandView(
    main::input::CommandInputContent command_input_content,
    std::unique_ptr<mod::Command> command)
    : 
    command_input_content_(std::move(command_input_content)),
    command_(std::move(command)) {

    ZAF_EXPECT(command_);
}


void PreservedCommandView::AfterParse() {

    __super::AfterParse();

    InitializeToolbar();

    commandEdit->SetStyle(CommandDisplayStyle::Preserved);
    commandEdit->SetInputContent(command_input_content_);

    UpdateCommandState();

    Subscriptions() += command_->StateUpdatedEvent().Subscribe([this](mod::Command*) {
        UpdateCommandState();
        state_updated_event_.Raise(zaf::As<PreservedCommandView>(shared_from_this()));
    });
}


void PreservedCommandView::InitializeToolbar() {

    Subscriptions() += MouseEnterEvent().Subscribe(std::bind([this]() {
        toolbar->SetIsVisible(true);
    }));

    Subscriptions() += MouseLeaveEvent().Subscribe(std::bind([this]() {
        if (!this->ContainMouse()) {
            toolbar->SetIsVisible(false);
        }
    }));

    Subscriptions() += toolbar->CloseEvent().Subscribe(std::bind([this]() {
        close_event_.Raise(zaf::As<PreservedCommandView>(shared_from_this()));
    }));

    Subscriptions() += toolbar->ExecuteEvent().Subscribe(std::bind([this]() {
        command_->GetExecutor()->Execute();
    }));
}


void PreservedCommandView::UpdateCommandState() {

    auto new_preview_control = CreateCommandPreviewControl(*command_);
    new_preview_control->SetStyle(CommandDisplayStyle::Preserved);
    new_preview_control->SetIsVisible(true);

    std::shared_ptr<mod::CommandPreviewControl> old_preview_control;
    if (previewContainer->ChildCount() > 0) {
        old_preview_control = zaf::As<mod::CommandPreviewControl>(
            previewContainer->Children().front());
    }

    if (new_preview_control != old_preview_control) {
        previewContainer->RemoveAllChildren();
        previewContainer->AddChild(new_preview_control);
    }

    toolbar->UpdateStyle(CommandDisplayStyle::Preserved, command_->GetExecutor());
}

}