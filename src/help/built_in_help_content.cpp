#include "help/built_in_help_content.h"
#include <format>
#include <zaf/base/as.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/resource/resource_factory.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/element/header_element.h"
#include "help/markdown/parse/markdown_parser.h"
#include "built_in_help_content.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

namespace ra::help {

std::shared_ptr<markdown::element::Element> LoadBuiltInHelpContent(std::wstring_view command) {

    try {

        auto uri = std::format(L"res:///CMD-{}.md", command);
        auto stream = zaf::ResourceFactory::Instance().LoadURI(uri);

        //TODO: Should not copy memory.
        std::string file_content;
        file_content.resize(static_cast<std::size_t>(stream.GetLength()), 0);
        stream.Read(stream.GetLength(), &file_content[0]);

        auto input = zaf::FromUTF8String(file_content);
        auto root_element = MarkdownParser::Instance()->Parse(input);

        //Remove content before header1.
        ElementList new_children;
        bool has_found_header1{};
        for (const auto& each_element : root_element->Children()) {

            if (each_element->Type() == ElementType::Header) {
                auto& header_element = zaf::As<HeaderElement>(*each_element);
                if (header_element.Depth() == HeaderDepth::_1) {
                    has_found_header1 = true;
                }
            }

            if (has_found_header1) {
                new_children.push_back(each_element);
            }
        }

        return MakeRoot(std::move(new_children));
    }
    catch (const zaf::Error&) {
        return nullptr;
    }
}

}