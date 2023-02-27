#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/error/error_command_parse_result.h"
#include "module/tool/error/error_preview_control.h"
#include "utility/command_line.h"

namespace ra::module::tool::error {

class ErrorCommand : public Command {
public:
    static CommandBrief GetBrief();
    static std::optional<ErrorCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    explicit ErrorCommand(const utility::CommandLine& command_line);

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::optional<ErrorCommandParseResult> parse_result_;
    std::shared_ptr<ErrorPreviewControl> preview_control_;
};

}