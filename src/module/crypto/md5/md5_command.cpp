#include "module/crypto/md5/md5_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::crypto {

MD5Command::MD5Command(const MD5CommandParseResult& parse_result) : parse_result_(parse_result) {

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