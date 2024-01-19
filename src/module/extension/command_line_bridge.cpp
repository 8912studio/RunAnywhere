#include "module/extension/command_line_bridge.h"
#include <zaf/base/range.h>

namespace ra::mod::extension {

CommandLineBridge::CommandLineBridge(const utility::CommandLine& command_line) {

    command_ = command_line.Command();
    arguments_ = command_line.Arguments();

    BuildPlainStruct();
}


void CommandLineBridge::BuildPlainStruct() {

    plain_arguments_ = std::make_unique<RA_CommandLinePiece[]>(arguments_.size());

    for (auto index : zaf::Range(0, arguments_.size())) {

        const auto& argument = arguments_[index];
        auto& plain_argument = plain_arguments_[index];

        plain_argument.type = static_cast<RA_CommandLinePieceType>(argument.Type());
        plain_argument.content = argument.Content().c_str();
    }

    plain_command_line_.command = command_.c_str();
    plain_command_line_.arguments = plain_arguments_.get();
    plain_command_line_.argument_count = static_cast<int32_t>(arguments_.size());
}

}