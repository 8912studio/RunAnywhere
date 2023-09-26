#pragma once

#include "module/tool/text_transform/text_transformer.h"

namespace ra::mod::tool::text_transform {

class LowerTransformer : public TextTransformer {
public:
    static CommandBrief Brief();

public:
    CommandBrief GetBrief() override;
    std::shared_ptr<help::markdown::element::Element> GetHelpContent() override;
    void Transform(std::wstring& text) override;
};

}