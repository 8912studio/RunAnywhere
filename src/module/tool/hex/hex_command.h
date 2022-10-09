#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/hex/hex_command_parse_result.h"
#include "module/tool/hex/hex_preview_control.h"
#include "utility/command_line.h"

namespace ra::module::tool::hex {

class HexCommand : public Command {
public:
    static CommandBrief GetBrief();

    static std::optional<HexCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    HexCommand(const utility::CommandLine& command_line);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::shared_ptr<HexPreviewControl> CreatePreviewControl() const;

private:
    std::optional<HexCommandParseResult> parse_result_;
    std::shared_ptr<HexPreviewControl> preview_control_;
};

}