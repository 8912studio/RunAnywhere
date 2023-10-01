#include "help/help_content_building.h"
#include <zaf/base/container/utility/sort.h>
#include "help/markdown/element/factory.h"

using namespace ra::help::markdown::element;

namespace ra::help {

std::shared_ptr<Element> BuildHelpContentFromSuggestedCommands(
    std::vector<mod::CommandBrief>&& commands) {

    ElementList block_elements;

    if (commands.empty()) {
        block_elements.push_back(MakeParagraph(L"No suggestions"));
    }
    else if (commands.size() == 1) {

        block_elements.push_back(MakeHeader(HeaderDepth::_2, {
            MakeInlineCode(commands.front().Command()),
        }));

        block_elements.push_back(MakeParagraph(commands.front().Description()));
    }
    else {

        zaf::Sort(commands, [](const auto& command1, const auto& command2) {
            return command1.Command() < command2.Command();
        });

        for (const auto& each_command : commands) {

            block_elements.push_back(MakeParagraph({
                MakeInlineCode(each_command.Command()),
                MakeText(L"  " + each_command.Description()),
            }));
        }
    }

    return MakeRoot(block_elements);
}

}