#include <main/history/history_command_view.h>
#include <zaf/object/type_definition.h>
#include "main/preview_control_creating.h"

namespace ra {

ZAF_DEFINE_TYPE(HistoryCommandView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/history/history_command_view.xaml")
ZAF_DEFINE_TYPE_END;

HistoryCommandView::HistoryCommandView(
    utility::CommandLine command_line, 
    std::unique_ptr<mod::Command> command)
    : 
    command_line_(std::move(command_line)),
    command_(std::move(command)) {

    ZAF_EXPECT(command_);
}


void HistoryCommandView::AfterParse() {

    __super::AfterParse();

    commandEdit->SetText(command_line_.RawText());

    auto preview_control = CreateCommandPreviewControl(*command_);
    preview_control->SetStyle(mod::PreviewStyle::Historical);
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
        close_event_.Raise(zaf::As<HistoryCommandView>(shared_from_this()));
    }));
}

}