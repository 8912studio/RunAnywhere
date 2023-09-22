#include "help/command_help_content_factory.h"
#include <format>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/resource/resource_factory.h>
#include "help/markdown/parse/markdown_parser.h"

using namespace ra::help::markdown::parse;

namespace ra::help {

CommandHelpContentFactory::CommandHelpContentFactory(std::wstring command) :
    command_(std::move(command)) {

}


std::shared_ptr<markdown::element::Element> CommandHelpContentFactory::LoadHelpContent() const {

    auto uri = std::format(L"res:///CMD-{}.md", command_);
    auto stream = zaf::ResourceFactory::Instance().LoadURI(uri);

    //TODO: Should not copy memory.
    std::string file_content;
    file_content.resize(static_cast<std::size_t>(stream.GetLength()), 0);
    stream.Read(stream.GetLength(), &file_content[0]);

    auto input = zaf::FromUTF8String(file_content);
    return MarkdownParser::Instance()->Parse(input);
}

}