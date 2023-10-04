#include "module/tool/hash/hash_command.h"
#include <zaf/creation.h>
#include <zaf/crypto/md5.h>
#include "module/active_path/active_path_modifying.h"
#include "module/common/copy_executor.h"
#include "module/tool/hash/hash_command_parsing.h"

namespace ra::mod::tool::hash {

bool HashCommand::Interpret(
	const utility::CommandLine& command_line,
	const context::DesktopContext& desktop_context,
	bool is_reusing) {

	//Not allow to reuse.
	if (is_reusing) {
		return false;
	}

	parse_result_ = ParseCommand(command_line);
	desktop_context_ = desktop_context;
	return true;
}


HashCommandParseResult HashCommand::ParseCommand(const utility::CommandLine& command_line) {
	return ParseHashCommand(command_line);
}


std::shared_ptr<CommandPreviewControl> HashCommand::GetPreviewControl() {

	if (!preview_control_) {
		CreatePreviewControl();
	}

	return preview_control_;
}


void HashCommand::CreatePreviewControl() {

	preview_control_ = zaf::Create<HashPreviewControl>(GetHashAlgorithmInfo());
	preview_control_->SetUseUppercase(parse_result_.use_uppercase);
	preview_control_->ShowHash(MakeGeneralInput(desktop_context_, parse_result_.general_option));
}


std::shared_ptr<CommandExecutor> HashCommand::GetExecutor() {

	if (!preview_control_) {
		return nullptr;
	}

	return CopyExecutor::TryCreate(preview_control_->GetText());
}

}