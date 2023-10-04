#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/hash/hash_command_parse_result.h"
#include "module/tool/hash/hash_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::hash {

class HashCommand : public Command {
public:
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

protected:
    virtual HashCommandParseResult ParseCommand(const utility::CommandLine& command_line);
    virtual HashAlgorithmInfo GetHashAlgorithmInfo() = 0;

private:
    void CreatePreviewControl();

private:
    HashCommandParseResult parse_result_;
    std::shared_ptr<HashPreviewControl> preview_control_;
    context::DesktopContext desktop_context_;
};

}