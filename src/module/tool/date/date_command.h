#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/date/date_command_parse_result.h"
#include "module/tool/date/date_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::date {

class DateCommand : public Command {
public:
    static CommandBrief Brief();

    static std::optional<DateCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::optional<DateCommandParseResult> parse_result_;
    std::shared_ptr<DatePreviewControl> preview_control_;
};

}