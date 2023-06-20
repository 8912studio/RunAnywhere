#pragma once

#include <optional>
#include "module/command.h"
#include "module/common/text_preview_control.h"
#include "module/tool/base64/base64_command_parsing.h"

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
    std::shared_ptr<CommandPreviewControl> CreatePreviewControl(
        const Base64CommandParseResult& parse_result);

    std::shared_ptr<CommandPreviewControl> CreateEncodePreviewControl(
        const Base64CommandParseResult& parse_result);

    std::shared_ptr<CommandPreviewControl> CreateDecodePreviewControl(
        const std::vector<std::byte>& decoded_data,
        const Base64CommandParseResult& parse_result);

private:
    std::shared_ptr<CommandPreviewControl> preview_control_;
};

}