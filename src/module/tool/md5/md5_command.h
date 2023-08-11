#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/md5/md5_command_parse_result.h"
#include "module/tool/md5/md5_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::md5 {

class MD5Command : public Command {
public:
    static CommandBrief Brief();

    static MD5CommandParseResult Parse(const utility::CommandLine& command_line);

public:
    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    void CreatePreviewControl();

private:
    MD5CommandParseResult parse_result_;
    std::shared_ptr<MD5PreviewControl> preview_control_;
    context::DesktopContext desktop_context_;
};

}