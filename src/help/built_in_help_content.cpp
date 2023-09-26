#include "help/built_in_help_content.h"
#include <format>
#include <zaf/base/as.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/resource/resource_factory.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/markdown_parser.h"
#include "built_in_help_content.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

namespace ra::help {
namespace {

std::shared_ptr<markdown::element::Element> LoadMarkdownFromResource(std::wstring_view command) {

    try {

        auto uri = std::format(L"res:///CMD-{}.md", command);
        auto stream = zaf::ResourceFactory::Instance().LoadURI(uri);

        //TODO: Should not copy memory.
        std::string file_content;
        file_content.resize(static_cast<std::size_t>(stream.GetLength()), 0);
        stream.Read(stream.GetLength(), &file_content[0]);

        auto input = zaf::FromUTF8String(file_content);
        return MarkdownParser::Instance()->Parse(input);
    }
    catch (const zaf::Error&) {
        return nullptr;
    }
}


ElementList RemoveContentBeforeHeader(const ElementList& elements) {

    ElementList result;
    bool has_found_header1{};

    for (const auto& each_element : elements) {

        if (each_element->Type() == ElementType::Header) {
            has_found_header1 = true;
        }

        if (has_found_header1) {
            result.push_back(each_element);
        }
    }

    return result;
}

}

std::shared_ptr<markdown::element::Element> LoadBuiltInHelpContent(std::wstring_view command) {

    auto root_element = LoadMarkdownFromResource(command);
    if (!root_element) {
        return nullptr;
    }

    auto new_children = RemoveContentBeforeHeader(root_element->Children());
    return MakeRoot(std::move(new_children));
}

}