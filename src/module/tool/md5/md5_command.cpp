#include "module/tool/md5/md5_command.h"
#include <zaf/creation.h>
#include "module/common/copy_executor.h"

namespace ra::mod::tool::md5 {

CommandBrief MD5Command::Brief() {
	return CommandBrief{
		L"md5",
		L"Calculate MD5 hash"
	};
}


MD5CommandParseResult MD5Command::Parse(const utility::CommandLine& command_line) {

	MD5CommandParseResult result;

	for (const auto& each_argument : command_line.Arguments()) {

		if (each_argument.empty()) {
			continue;
		}

		if (each_argument.front() == L'/') {

			auto switch_value = each_argument.substr(1);
			if (switch_value == L"u8") {
				result.encoding = TextEncoding::UTF8;
			}
			else if (switch_value == L"u16") {
				result.encoding = TextEncoding::UTF16;
			}
			else if (switch_value == L"c") {
				result.use_uppercase = true;
			}
		}
		else {
			result.string = each_argument;
		}
	}

	return result;
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

	parse_result_ = Parse(command_line);
	desktop_context_ = desktop_context;
	return true;
}


std::shared_ptr<CommandPreviewControl> MD5Command::GetPreviewControl() {

	if (!preview_control_) {

		preview_control_ = zaf::Create<MD5PreviewControl>();
		preview_control_->SetUseUppercase(parse_result_.use_uppercase);

		if (!parse_result_.string.empty()) {
			preview_control_->ShowStringMD5(parse_result_.string, parse_result_.encoding);
		}
		else {
			preview_control_->ShowFileMD5(desktop_context_.active_path.GetPath());
		}
	}

	return preview_control_;
}


std::shared_ptr<CommandExecutor> MD5Command::GetExecutor() {

	if (!preview_control_) {
		return nullptr;
	}

	return CopyExecutor::TryCreate(preview_control_->GetText());
}

}