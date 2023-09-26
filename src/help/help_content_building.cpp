#include "help/help_content_building.h"
#include <zaf/base/container/utility/sort.h>
#include "help/markdown/element/factory.h"

using namespace ra::help::markdown::element;

namespace ra::help {

std::shared_ptr<Element> BuildHelpContentFromSuggestedCommands(
    std::vector<mod::CommandBrief>&& commands) {

    ElementList block_elements;
    block_elements.push_back(MakeHeader(HeaderDepth::_1, L"Suggestions"));

    zaf::Sort(commands, [](const auto& command1, const auto& command2) {
        return command1.Command() < command2.Command();
    });

    for (const auto& each_command : commands) {

        block_elements.push_back(MakeParagraph({
            MakeInlineCode(each_command.Command()),
            MakeText(L"  " + each_command.Description()),
        }));
    }

    if (block_elements.empty()) {
        block_elements.push_back(MakeParagraph(L"No suggestions"));
    }

    return MakeRoot(block_elements);
}

}