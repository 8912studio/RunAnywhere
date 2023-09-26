#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/hex/hex_command_parse_result.h"
#include "module/tool/hex/hex_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::hex {

class HexCommand : public Command {
public:
    static CommandBrief Brief();

public:
    CommandBrief GetBrief() override;
    std::shared_ptr<help::markdown::element::Element> GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    void CreatePreviewControl();

private:
    std::optional<HexCommandParseResult> parse_result_;
    std::shared_ptr<HexPreviewControl> preview_control_;
    context::DesktopContext desktop_context_;
};

}