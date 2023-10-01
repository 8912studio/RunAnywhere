#include "module/command_brief.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"

namespace ra::mod {

CommandBrief::CommandBrief(
    std::wstring keyword,
    std::shared_ptr<help::markdown::element::Element> description)
    :
    keyword_(std::move(keyword)),
    description_(std::move(description)) {

    ZAF_EXPECT(description_);
    ZAF_EXPECT(description_->Type() == help::markdown::element::ElementType::Paragraph);
}


CommandBrief::CommandBrief(std::wstring keyword, std::wstring description) :
    keyword_(std::move(keyword)) {

    description_ = help::markdown::element::MakeParagraph(std::move(description));
}

}