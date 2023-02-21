#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/image_box.h>
#include <zaf/control/label.h>
#include <zaf/window/dialog.h>
#include "module/user_defined/import/bundle_importer.h"

namespace ra::module::user_defined {

class ImportBundleWindow : public zaf::Dialog {
public:
    ZAF_DECLARE_TYPE

    ImportBundleWindow();
    explicit ImportBundleWindow(const std::shared_ptr<BundleImporter>& importer);

protected:
    void AfterParse() override;
    void OnHandleCreated(const zaf::HandleCreatedInfo& event_info) override;

private:
    enum class ButtonsStyle {
        OK,
        OKCancel,
        YesNo,
    };

    class ImportStateDisplayInfo {
    public:
        std::wstring icon_uri;
        std::wstring title_text;
        std::wstring message_text;
        zaf::TextTrimming message_text_trimming;
        ButtonsStyle buttons_style{ ButtonsStyle::OK };
    };

private:
    void InitializeControls();
    void ShowImportState();
    void ShowButtons(ButtonsStyle style);
    ImportStateDisplayInfo GetPendingStateDisplayInfo() const;
    ImportStateDisplayInfo GetSuccessStateDisplayInfo() const;
    ImportStateDisplayInfo GetOverrideConfirmStateDisplayInfo() const;
    ImportStateDisplayInfo GetConflictConfirmStateDisplayInfo() const;
    void GetConflictMessageDisplayInfo(ImportStateDisplayInfo& info) const;
    ImportStateDisplayInfo GetErrorStateDisplayInfo() const;
    void GetErrorMessageDisplayInfo(ImportStateDisplayInfo& info) const;
    void GetParseErrorMessageDisplayInfo(ImportStateDisplayInfo& info) const;
    void UpdateWindowHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, container);
    ZAF_BIND_CONTROL(zaf::ImageBox, iconImage);
    ZAF_BIND_CONTROL(zaf::Label, titleLabel);
    ZAF_BIND_CONTROL(zaf::Label, messageLabel);
    ZAF_BIND_CONTROL(zaf::Button, okButton);
    ZAF_BIND_CONTROL(zaf::Button, cancelButton);

    std::shared_ptr<BundleImporter> importer_;
};

}