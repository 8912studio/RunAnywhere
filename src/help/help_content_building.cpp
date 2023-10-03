#include "help/help_content_building.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/container/utility/sort.h>
#include "help/markdown/element/factory.h"

using namespace ra::help::markdown::element;

namespace ra::help {

std::shared_ptr<markdown::element::Element> GetNoHelpContentParagraph() {
    return MakeParagraph({ MakeItalic(L"No help content.") });
}


std::shared_ptr<markdown::element::Element> BuildDescriptionParagraph(std::wstring description) {
    if (!description.empty()) {
        return MakeParagraph(std::move(description));
    }
    return GetNoHelpContentParagraph();
}


std::shared_ptr<Element> BuildHelpContentFromSuggestedCommands(
    std::vector<mod::CommandBrief>&& commands) {

    ElementList block_elements;

    if (commands.empty()) {
        block_elements.push_back(MakeParagraph({ MakeItalic(L"No suggestions") }));
    }
    else if (commands.size() == 1) {

        block_elements.push_back(MakeHeader(HeaderDepth::_2, {
            MakeInlineCode(commands.front().Keyword()),
        }));

        block_elements.push_back(commands.front().Description());
    }
    else {

        zaf::Sort(commands, [](const auto& command1, const auto& command2) {
            return command1.Keyword() < command2.Keyword();
        });

        for (const auto& each_command : commands) {

            ElementList new_children;
            new_children.push_back(MakeInlineCode(each_command.Keyword()));
            new_children.push_back(MakeText(L"  "));
            zaf::Append(new_children, each_command.Description()->Children());

            block_elements.push_back(MakeParagraph(std::move(new_children)));
        }
    }

    return MakeRoot(block_elements);
}

}