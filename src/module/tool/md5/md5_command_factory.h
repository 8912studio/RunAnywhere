#pragma once

#include "module/tool/md5/md5_command_parse_result.h"
#include "module/tool/tool_command_factory.h"

namespace ra::module::tool::md5 {

class MD5CommandFactory : public ToolCommandFactory {
public:
    static std::optional<MD5CommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    MD5CommandFactory();

    std::shared_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;
};

}