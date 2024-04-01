#include "module/command_brief.h"
#include <zaf/base/error/contract.h>
#include "utility/markdown/element/factory.h"

namespace ra::mod {

CommandBrief::CommandBrief(
    std::wstring keyword,
    std::shared_ptr<utility::markdown::element::Element> description)
    :
    keyword_(std::move(keyword)),
    description_(std::move(description)) {

    ZAF_EXPECT(description_);
    ZAF_EXPECT(description_->Type() == utility::markdown::element::ElementType::Paragraph);
}


CommandBrief::CommandBrief(std::wstring keyword, std::wstring description) :
    keyword_(std::move(keyword)) {

    description_ = utility::markdown::element::MakeParagraph(std::move(description));
}

}