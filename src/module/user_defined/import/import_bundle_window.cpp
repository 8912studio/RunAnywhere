#include "module/user_defined/import/import_bundle_window.h"
#include <cassert>
#include <zaf/base/error/check.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>
#include "module/user_defined/bundle_definition.h"

namespace ra::mod::user_defined {

ZAF_DEFINE_TYPE(ImportBundleWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/user_defined/import/import_bundle_window.xaml");
ZAF_DEFINE_TYPE_END


ImportBundleWindow::ImportBundleWindow() {
    assert(false);
}


ImportBundleWindow::ImportBundleWindow(const std::shared_ptr<BundleImporter>& importer) : 
    importer_(importer) {

}


void ImportBundleWindow::AfterParse() {

    __super::AfterParse();

    iconImage->SetImageLayout(zaf::ImageLayout::Stretch);

    InitializeControls();
}


void ImportBundleWindow::OnHandleCreated(const zaf::HandleCreatedInfo& event_info) {

    __super::OnHandleCreated(event_info);

    ShowImportState();
}


void ImportBundleWindow::InitializeControls() {

    Subscriptions() += okButton->ClickEvent().Subscribe(std::bind([this]() {

        auto state = importer_->GetState();
        if (state == BundleImporter::State::Pending) {

            importer_->Import();
            ShowImportState();
        }
        else if (state == BundleImporter::State::OverrideConfirm ||
                 state == BundleImporter::State::ConflictConfirm) {

            importer_->Confirm();
            ShowImportState();
        }
        else {
            this->Close();
        }
    }));

    Subscriptions() += cancelButton->ClickEvent().Subscribe(std::bind([this]() {
        this->Close();
    }));

    Subscriptions() += container->RectChangedEvent().Subscribe(
        std::bind(&ImportBundleWindow::UpdateWindowHeight, this));
}


void ImportBundleWindow::ShowImportState() {

    ImportBundleWindow::ImportStateDisplayInfo display_info;

    switch (importer_->GetState()) {
    case BundleImporter::State::Pending:
        display_info = GetPendingStateDisplayInfo();
        break;
    case BundleImporter::State::Success:
        display_info = GetSuccessStateDisplayInfo();
        break;
    case BundleImporter::State::OverrideConfirm:
        display_info = GetOverrideConfirmStateDisplayInfo();
        break;
    case BundleImporter::State::ConflictConfirm:
        display_info = GetConflictConfirmStateDisplayInfo();
        break;
    case BundleImporter::State::Fail:
        display_info = GetErrorStateDisplayInfo();
        break;
    default:
        ZAF_NOT_REACHED();
    }

    iconImage->SetURI(display_info.icon_uri);
    titleLabel->SetText(display_info.title_text);
    messageLabel->SetText(display_info.message_text);
    messageLabel->SetIsVisible(!display_info.message_text.empty());

    ShowButtons(display_info.buttons_style);
}


void ImportBundleWindow::ShowButtons(ButtonsStyle style) {

    std::wstring ok_button_text;
    std::wstring cancel_button_text;
    bool is_cancel_button_visible{ true };

    switch (style) {
    case ButtonsStyle::OK:
        is_cancel_button_visible = false;
        [[fallthrough]];
    case ButtonsStyle::OKCancel:
        ok_button_text = L"OK";
        cancel_button_text = L"Cancel";
        break;
    case ButtonsStyle::YesNo:
        ok_button_text = L"Yes";
        cancel_button_text = L"No";
        break;
    default:
        ZAF_NOT_REACHED();
    }

    okButton->SetText(ok_button_text);
    cancelButton->SetText(cancel_button_text);
    cancelButton->SetIsVisible(is_cancel_button_visible);

    this->SetDefaultButton(is_cancel_button_visible ? cancelButton : okButton);
}


ImportBundleWindow::ImportStateDisplayInfo ImportBundleWindow::GetPendingStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/question.png";
    result.title_text =
        L"Confirm to import " +
        importer_->GetBundlePath().filename().wstring() +
        L"?";
    result.buttons_style = ButtonsStyle::YesNo;
    return result;
}


ImportBundleWindow::ImportStateDisplayInfo ImportBundleWindow::GetSuccessStateDisplayInfo() const {
    
    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/info.png";
    result.title_text = 
        L"Import " +
        importer_->GetBundlePath().filename().wstring() +
        L" succeeded";
    result.buttons_style = ButtonsStyle::OK;
    return result;
}


ImportBundleWindow::ImportStateDisplayInfo
    ImportBundleWindow::GetOverrideConfirmStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/warn.png";
    result.title_text = importer_->GetBundlePath().filename().wstring() + L" already exists";
    result.message_text = L"Confirm to override it?";
    result.buttons_style = ButtonsStyle::YesNo;
    return result;
}


ImportBundleWindow::ImportStateDisplayInfo 
    ImportBundleWindow::GetConflictConfirmStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/warn.png";

    result.title_text = 
        importer_->GetBundlePath().filename().wstring() + 
        L" has conflicted entries";

    GetConflictMessageDisplayInfo(result);

    result.buttons_style = ButtonsStyle::YesNo;
    return result;
}


void ImportBundleWindow::GetConflictMessageDisplayInfo(ImportStateDisplayInfo& info) const {

    info.message_text = L"They are:\n";
    
    constexpr std::size_t max_display_entries = 4;
    std::size_t current_display_entries{};
    for (const auto& each_entries : importer_->GetConflictEntries()) {

        info.message_text += L'\"';
        info.message_text += each_entries->Keyword();
        info.message_text += L'\"';
        info.message_text += L" in ";
        info.message_text += each_entries->BundleMeta()->BundleID() + BundleFileExtension;
        info.message_text += L'\n';

        current_display_entries++;
        if (current_display_entries >= max_display_entries) {
            break;
        }
    }

    if (importer_->GetConflictEntries().size() > max_display_entries) {
        info.message_text += L"...\n";
    }

    info.message_text += L"Confirm to import anyway?";

    info.message_text_trimming.SetGranularity(zaf::TextTrimmingGranularity::Character);
}


ImportBundleWindow::ImportStateDisplayInfo ImportBundleWindow::GetErrorStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/error.png";

    result.title_text = 
        L"Import " +
        importer_->GetBundlePath().filename().wstring() +
        L" failed";

    GetErrorMessageDisplayInfo(result);

    result.buttons_style = ButtonsStyle::OK;
    return result;
}


void ImportBundleWindow::GetErrorMessageDisplayInfo(ImportStateDisplayInfo& info) const {

    switch (importer_->GetFailReason()) {

    case BundleImporter::FailReason::CannotOpenFile:
        info.message_text = L"Unable to open file";
        break;

    case BundleImporter::FailReason::ParseError:
        GetParseErrorMessageDisplayInfo(info);
        break;

    case BundleImporter::FailReason::NeedRetry:
        info.message_text = L"Please try again";
        break;

    case BundleImporter::FailReason::NeedReload:
        info.message_text = L"Please run \"!reload\" and try again";
        break;

    case BundleImporter::FailReason::SaveError:
        info.message_text = L"Unable to copy file. Error code: ";
        info.message_text += std::to_wstring(importer_->GetSaveError()->value());
        break;

    default:
        info.message_text = L"Unknown error";
        break;
    }
}


void ImportBundleWindow::GetParseErrorMessageDisplayInfo(ImportStateDisplayInfo& info) const {

    info.message_text = L"There is syntax error at line ";
    info.message_text += std::to_wstring(importer_->GetParseError()->ErrorLineNumber());
    info.message_text += L":\n";
    info.message_text += L'\"';
    info.message_text += zaf::FromUTF8String(importer_->GetParseError()->ErrorLine());
    info.message_text += L'\"';

    info.message_text_trimming.SetGranularity(zaf::TextTrimmingGranularity::Character);
    info.message_text_trimming.SetDelimiter(L'\"');
    info.message_text_trimming.SetDelimiterCount(1);
}


void ImportBundleWindow::UpdateWindowHeight() {

    float total_height{};
    for (const auto& each_child : RootControl()->Children()) {
        total_height += each_child->Height();
        total_height += each_child->Margin().Height();
    }

    this->SetContentHeight(total_height);
}

}