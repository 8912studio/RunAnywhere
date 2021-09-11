#include "module/crypto/md5/md5_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::crypto {

MD5Command::MD5Command(const MD5CommandParseResult& parse_result) : parse_result_(parse_result) {

}


help::content::Content MD5Command::GetHelpContent() {

	help::content::Content result;

	result.AddTitleLine(L"MD5 command");
	result.AddBodyLine(L"Calculate MD5 hash of activated file's content or specified text.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"md5 [text] [switches]");
	result.AddBodyLine(L"If `text` is omitted, activated file's content would be calculated.");
	result.AddBodyLine(
		L"If there are space characters in `text`, enclose it in double quotation marks( \" ).");

	result.AddTitleLine(L"Switches");
	result.AddTwoPartsLine(L"/u8", L"Calculate `text` in UTF-8 encoding. This is default.");
	result.AddTwoPartsLine(L"/u16", L"Calculate `text` in UTF-16 encoding.");
	result.AddTwoPartsLine(L"/c", L"Use upper-case in result.");

	return result;
}


std::shared_ptr<CommandPreviewControl> MD5Command::GetPreviewControl() {

	if (!preview_control_) {

		preview_control_ = zaf::Create<MD5PreviewControl>();
		preview_control_->SetUseUppercase(parse_result_.use_uppercase);

		if (!parse_result_.string.empty()) {
			preview_control_->ShowStringMD5(parse_result_.string, parse_result_.encoding);
		}
		else {
			preview_control_->ShowFileMD5(GetDesktopContext().active_path.GetPath());
		}
	}

	return preview_control_;
}


void MD5Command::Execute() {

	if (!preview_control_) {
		return;
	}

	auto text = preview_control_->GetText();
	if (text.empty()) {
		return;
	}

	utility::SetStringToClipboard(text);
}

}