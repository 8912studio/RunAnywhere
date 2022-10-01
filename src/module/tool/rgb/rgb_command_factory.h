#pragma once

#include "module/tool/rgb/rgb_command_parse_result.h"
#include "module/tool/tool_command_factory.h"
#include "utility/command_line.h"

namespace ra::module::tool::rgb {

class RGBCommandFactory : public ToolCommandFactory {
public:
    static std::optional<RGBCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    RGBCommandFactory();

    std::shared_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;
};

}