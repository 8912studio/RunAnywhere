#include "help/built_in_help_content_manager.h"
#include <format>
#include <zaf/base/as.h>
#include <zaf/base/error/check.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/resource/resource_factory.h>
#include "help/help_content_building.h"
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/markdown_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

namespace ra::help {

BuiltInHelpContentManager& BuiltInHelpContentManager::Instance() {
    static BuiltInHelpContentManager instance;
    return instance;
}


std::shared_ptr<markdown::element::Element> BuiltInHelpContentManager::GetDescription(
    const std::wstring& command) {

    return GetHelpContent(command).description;
}


std::shared_ptr<Element> BuiltInHelpContentManager::GetDetail(const std::wstring& command) {
    return GetHelpContent(command).detail;
}


const BuiltInHelpContentManager::HelpContent& BuiltInHelpContentManager::GetHelpContent(
    const std::wstring& command) {

    auto iterator = map_.find(command);
    if (iterator != map_.end()) {
        return iterator->second;
    }

    auto new_detail = LoadBuiltInHelpContent(command);
    if (!new_detail) {

        new_detail = MakeRoot({
            MakeHeader(HeaderDepth::_2, { MakeInlineCode(command) }),
            GetNoHelpContentParagraph()
        });
    }

    HelpContent result;
    result.detail = new_detail;
    result.description = GetDescriptionFromDetail(*new_detail);

    auto insert_result = map_.insert(std::make_pair(command, std::move(result)));
    return insert_result.first->second;
}


std::shared_ptr<Element> BuiltInHelpContentManager::LoadBuiltInHelpContent(
    const std::wstring& command) {

    auto root_element = LoadMarkdownFromResource(command);
    if (!root_element) {
        return nullptr;
    }

    auto new_children = RemoveContentBeforeHeader(root_element->Children());
    return MakeRoot(std::move(new_children));
}


std::shared_ptr<Element> BuiltInHelpContentManager::LoadMarkdownFromResource(
    const std::wstring& command) {

    try {

        auto uri = std::format(L"res:///CMD-{}.md", command);
        auto stream = zaf::ResourceFactory::Instance().LoadURI(uri);

        //TODO: Should not copy memory.
        std::string file_content;
        file_content.resize(static_cast<std::size_t>(stream.GetSize()), 0);
        stream.Read(stream.GetSize(), &file_content[0]);

        auto input = zaf::FromUTF8String(file_content);
        return MarkdownParser::Instance()->Parse(input);
    }
    catch (const zaf::Error&) {
        return nullptr;
    }
}


ElementList BuiltInHelpContentManager::RemoveContentBeforeHeader(const ElementList& elements) {

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


std::shared_ptr<markdown::element::Element> BuiltInHelpContentManager::GetDescriptionFromDetail(
    const markdown::element::Element& root) {

    ZAF_EXPECT(root.Type() == ElementType::Root);

    const auto& children = root.Children();
    ZAF_EXPECT(children.size() >= 2);
    ZAF_EXPECT(children[0]->Type() == ElementType::Header);
    ZAF_EXPECT(children[1]->Type() == ElementType::Paragraph);

    return children[1];
}

}