#pragma once

#include "module/tool/date/date_command_parse_result.h"
#include "module/tool/tool_command_factory.h"
#include "utility/command_line.h"

namespace ra::module::tool::date {

class DateCommandFactory : public ToolCommandFactory {
public:
    static std::optional<DateCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    DateCommandFactory();

    std::shared_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;
};

}