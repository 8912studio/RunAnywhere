#include "module/user_defined/import/import_bundle_window.h"
#include <cassert>
#include <zaf/base/assert.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>


namespace ra::module::user_defined{

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

    InitializeControls();

    importer_->Import();
    ShowImportState();
}


void ImportBundleWindow::InitializeControls() {

    Subscriptions() += okButton->ClickEvent().Subscribe(std::bind([this]() {

        auto state = importer_->GetState();
        if (state == BundleImporter::State::OverrideConfirm ||
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
}


void ImportBundleWindow::ShowImportState() {

    ImportBundleWindow::ImportStateDisplayInfo display_info;

    switch (importer_->GetState()) {
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
        ZAF_FAIL();
    }

    iconImage->SetURI(display_info.icon_uri);
    titleLabel->SetText(display_info.title_text);
    messageLabel->SetText(display_info.message_text);
    messageLabel->SetIsVisible(!display_info.message_text.empty());
    okButton->SetText(display_info.ok_button_text);
    cancelButton->SetIsVisible(display_info.is_cancel_button_visible);
    
    this->SetDefaultButton(display_info.is_cancel_button_visible ? cancelButton : okButton);

    UpdateWindowHeight();
}


ImportBundleWindow::ImportStateDisplayInfo ImportBundleWindow::GetSuccessStateDisplayInfo() const {
    
    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/info.png";

    result.title_text = 
        L"Import " +
        importer_->GetBundlePath().filename().wstring() +
        L" succeeded";

    result.ok_button_text = L"OK";
    result.is_cancel_button_visible = false;
    return result;
}


ImportBundleWindow::ImportStateDisplayInfo
    ImportBundleWindow::GetOverrideConfirmStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/warn.png";
    result.title_text = importer_->GetBundlePath().filename().wstring() + L" already exists";
    result.message_text = L"Confirm to override it?";
    result.ok_button_text = L"Confirm";
    result.is_cancel_button_visible = true;
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

    result.ok_button_text = L"Confirm";
    result.is_cancel_button_visible = true;
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
        info.message_text += each_entries->BundleMeta()->BundleID() + L".rabdl";
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

    info.message_text_trimming.granularity = zaf::TextTrimming::Granularity::Character;
}


ImportBundleWindow::ImportStateDisplayInfo ImportBundleWindow::GetErrorStateDisplayInfo() const {

    ImportStateDisplayInfo result;
    result.icon_uri = L"res:///resource/error.png";

    result.title_text = 
        L"Import " +
        importer_->GetBundlePath().filename().wstring() +
        L" failed";

    GetErrorMessageDisplayInfo(result);

    result.ok_button_text = L"OK";
    result.is_cancel_button_visible = false;
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
    info.message_text += zaf::FromUtf8String(importer_->GetParseError()->ErrorLine());
    info.message_text += L'\"';

    info.message_text_trimming.granularity = zaf::TextTrimming::Granularity::Character;
    info.message_text_trimming.delimiter = L'\"';
    info.message_text_trimming.delimiter_count = 1;
}


void ImportBundleWindow::UpdateWindowHeight() {

    float total_height{};
    for (const auto& each_child : RootControl()->Children()) {
        total_height += each_child->Height();
        total_height += each_child->Margin().Height();
    }

    total_height += RootControl()->Padding().Height();
    total_height += RootControl()->Margin().Height();

    this->SetContentHeight(total_height);
}

}