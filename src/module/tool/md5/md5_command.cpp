#include "module/tool/md5/md5_command.h"
#include <zaf/creation.h>
#include "module/active_path/active_path_modifying.h"
#include "module/common/copy_executor.h"
#include "module/tool/md5/md5_command_parsing.h"

namespace ra::mod::tool::md5 {

CommandBrief MD5Command::Brief() {
	return CommandBrief{
		L"md5",
		L"Calculate MD5 hash"
	};
}


CommandBrief MD5Command::GetBrief() {
	return Brief();
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
	result.AddBodyLine(L"Or use text block(press ALT+T to insert) to contain arbitrary text.");

	result.AddTitleLine(L"Switches");
	result.AddTwoPartsLine(L"/u8", L"Calculate `text` in UTF-8 encoding. This is default.");
	result.AddTwoPartsLine(L"/u16", L"Calculate `text` in UTF-16 encoding.");
	result.AddTwoPartsLine(L"/c", L"Use upper-case in result.");

	return result;
}


bool MD5Command::Interpret(
	const utility::CommandLine& command_line,
	const context::DesktopContext& desktop_context,
	bool is_reusing) {

	//Not allow to reuse.
	if (is_reusing) {
		return false;
	}

	parse_result_ = ParseMD5Command(command_line);
	desktop_context_ = desktop_context;
	return true;
}


std::shared_ptr<CommandPreviewControl> MD5Command::GetPreviewControl() {

	if (!preview_control_) {
		CreatePreviewControl();
	}

	return preview_control_;
}


void MD5Command::CreatePreviewControl() {

	preview_control_ = zaf::Create<MD5PreviewControl>();
	preview_control_->SetUseUppercase(parse_result_.use_uppercase);
	preview_control_->ShowMD5(MakeGeneralInput(desktop_context_, parse_result_.general_option));
}


std::shared_ptr<CommandExecutor> MD5Command::GetExecutor() {

	if (!preview_control_) {
		return nullptr;
	}

	return CopyExecutor::TryCreate(preview_control_->GetText());
}

}