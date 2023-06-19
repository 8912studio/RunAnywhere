#pragma once

#include <optional>
#include "module/command.h"
#include "module/tool/base64/base64_command_parsing.h"
#include "module/tool/text_transform/text_preview_control.h"

namespace ra::mod::tool::base64 {

class Base64Command : public Command {
public:
    static CommandBrief Brief();

public:
    explicit Base64Command(const utility::CommandLine&);

    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    std::shared_ptr<text_transform::TextPreviewControl> preview_control_;
};

}