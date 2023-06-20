#pragma once

#include <filesystem>
#include <optional>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"
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

private:
	enum class LayoutType {
		Progress,
		Error,
		Result,
	};

private:
	void SetSourceIcon(const std::wstring& uri);
	void SetMD5Encoding(std::optional<TextEncoding> encoding);
	void SetMD5Text(const std::wstring& md5);
	void ChangeLayout(LayoutType type);

private:
	ZAF_BIND_CONTROL(zaf::Control, md5SourceIcon);
	ZAF_BIND_CONTROL(zaf::Control, md5EncodingContainer);
	ZAF_BIND_CONTROL(zaf::Label, md5EncodingControl);
	ZAF_BIND_CONTROL(zaf::Label, md5SourceControl);
	ZAF_BIND_CONTROL(utility::ProgressCircle, progressCircle);
	ZAF_BIND_CONTROL(zaf::Control, errorControl);
	ZAF_BIND_CONTROL(utility::PreviewTextBox, md5ResultControl);

	bool use_upper_case_{};
};

}