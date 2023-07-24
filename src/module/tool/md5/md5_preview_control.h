#pragma once

#include <filesystem>
#include <optional>
#include <zaf/control/control_binder.h>
#include <zaf/control/image_box.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"
#include "module/common/content_status_bar.h"
#include "module/common/text_encoding.h"
#include "utility/preview_text_box.h"
#include "utility/progress_circle.h"

namespace ra::mod::tool::md5 {

class MD5PreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_TYPE;

	void ShowFileMD5(const std::filesystem::path& file_path);
	void ShowStringMD5(const std::wstring& string, TextEncoding encoding);

	std::wstring GetText();

	void SetUseUppercase(bool value) {
		use_upper_case_ = value;
	}

protected:
	void OnStyleChanged() override;

private:
	enum class LayoutType {
		Progress,
		Error,
		Result,
	};

private:
	void SetMD5Text(const std::wstring& md5);
	void AdjustControlStyles();
	void ChangeLayout(LayoutType type);

private:
	ZAF_BIND_CONTROL(ContentStatusBar, contentStatusBar);
	ZAF_BIND_CONTROL(utility::ProgressCircle, progressCircle);
	ZAF_BIND_CONTROL(zaf::HorizontalBox, errorControl);
	ZAF_BIND_CONTROL(zaf::ImageBox, errorIcon);
	ZAF_BIND_CONTROL(utility::PreviewTextBox, md5ResultControl);

	bool use_upper_case_{};
};

}