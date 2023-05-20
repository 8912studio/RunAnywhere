#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"

namespace ra::module::tool::text_transform {

class LowerCommand : public Command {
public:
    static CommandBrief Brief();

public:
    explicit LowerCommand(const utility::CommandLine& command_line);

    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::wstring lowered_text_;
};

}