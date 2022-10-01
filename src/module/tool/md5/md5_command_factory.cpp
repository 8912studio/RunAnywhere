#include "module/tool/md5/md5_command_factory.h"
#include "module/tool/md5/md5_command.h"

namespace ra::module::tool::md5 {
namespace {

CommandBrief GetCommandBrief() {
    return CommandBrief{
        L"md5",
        L"Calculate MD5 hash"
    };
}

}

std::optional<MD5CommandParseResult> MD5CommandFactory::Parse(
    const utility::CommandLine& command_line) {

	MD5CommandParseResult result;

	for (const auto& each_argument : command_line.Arguments()) {

		if (each_argument.empty()) {
			continue;
		}

		if (each_argument.front() == L'/') {

			auto switch_value = each_argument.substr(1);
			if (switch_value == L"u8") {
				result.encoding = MD5Encoding::UTF8;
			}
			else if (switch_value == L"u16") {
				result.encoding = MD5Encoding::UTF16;
			}
			else if (switch_value == L"c") {
				result.use_uppercase = true;
			}
		}
		else {
			result.string = each_argument;
		}
	}

	return result;
}


MD5CommandFactory::MD5CommandFactory() : ToolCommandFactory(GetCommandBrief()) {

}


std::shared_ptr<Command> MD5CommandFactory::CreateCommand(
    const utility::CommandLine& command_line) {

    auto parse_result = Parse(command_line);
    if (!parse_result) {
        return nullptr;
    }

    return std::make_shared<MD5Command>(*parse_result);
}

}