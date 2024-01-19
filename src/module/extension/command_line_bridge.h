#pragma once

#include <memory>
#include <zaf/base/non_copyable.h>
#include "module/extension/sdk/run_anywhere_extension_sdk.h"
#include "utility/command_line.h"

namespace ra::mod::extension {

class CommandLineBridge : zaf::NonCopyableNonMovable {
public:
    explicit CommandLineBridge(const utility::CommandLine& command_line);

    const RA_CommandLine* PlainStruct() const {
        return &plain_command_line_;
    }

private:
    void BuildPlainStruct();

private:
    std::wstring command_;
    std::vector<utility::CommandLinePiece> arguments_;

    RA_CommandLine plain_command_line_{};
    std::unique_ptr<RA_CommandLinePiece[]> plain_arguments_;
};

}