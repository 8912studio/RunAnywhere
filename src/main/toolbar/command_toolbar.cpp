#include "main/toolbar/command_toolbar.h"
#include <zaf/base/as.h>
#include "module/common/copy_executor.h"

namespace ra {

ZAF_OBJECT_IMPL(CommandToolbar);

void CommandToolbar::AfterParse() {

    __super::AfterParse();

    Subscriptions() += executeButton->ClickEvent().Subscribe(std::bind([this]() {
        execute_event_.Raise({});
    }));

    Subscriptions() += preserveButton->ClickEvent().Subscribe(std::bind([this]() {
        preserve_event_.Raise({});
    }));

    Subscriptions() += closeButton->ClickEvent().Subscribe(std::bind([this]() {
        close_event_.Raise({});
    }));
}


void CommandToolbar::UpdateStyle(
    CommandDisplayStyle style,
    const std::shared_ptr<mod::CommandExecutor>& executor) {

    UpdateExecuteButton(style, executor);
    UpdatePreserveButton(style);

    closeButton->SetIsVisible(style == CommandDisplayStyle::Preserved);
}


void CommandToolbar::UpdateExecuteButton(
    CommandDisplayStyle style,
    const std::shared_ptr<mod::CommandExecutor>& executor) {

    if (!executor) {
        executeButton->SetIsVisible(false);
        return;
    }

    std::wstring image_name;
    std::wstring tooltip;

    if (zaf::As<mod::CopyExecutor>(executor)) {
        image_name = L"copy";
        tooltip = L"Copy result to clipboard";
    }
    else {
        image_name = L"execute";
        tooltip = L"Execute command";
    }

    if (style == CommandDisplayStyle::Normal) {
        tooltip += L" (ENTER)";
    }

    executeButton->SetImageName(image_name);
    executeButton->SetTooltip(tooltip);
    executeButton->SetIsVisible(true);
}


void CommandToolbar::UpdatePreserveButton(CommandDisplayStyle style) {

    if (style == CommandDisplayStyle::Preserved) {
        preserveButton->SetIsVisible(false);
        return;
    }

    preserveButton->SetImageName(L"preserve");
    preserveButton->SetTooltip(L"Preserve command (SHIFT+ENTER)");
    preserveButton->SetIsVisible(true);
}

}