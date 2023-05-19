#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"

namespace ra::module::tool::text_transform {

class LowerCommand : public Command {
public:
    static CommandBrief GetBrief();

public:
    explicit LowerCommand(const utility::CommandLine& command_line);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::wstring lowered_text_;
};

}