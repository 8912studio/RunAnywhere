#include "help/help_content_building.h"
#include <zaf/base/container/utility/sort.h>

namespace ra::help {

content::Content BuildHelpContentFromSuggestedCommands(
    std::vector<mod::CommandBrief>&& commands) {

    help::content::Content help_content;
    help_content.AddTitleLine(L"Suggestions");

    if (commands.empty()) {
        help_content.AddBodyLine(L"No suggestions");
        return help_content;
    }

    zaf::Sort(
        commands,
        [](const mod::CommandBrief& command1, const mod::CommandBrief& command2) {
            return command1.Command() < command2.Command();
        }
    );

    for (const auto& each_command : commands) {
        help_content.AddTwoPartsLine(each_command.Command(), each_command.Description());
    }

    return help_content;
}

}