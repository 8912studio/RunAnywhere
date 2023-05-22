#pragma once

#include "module/tool/text_transform/text_transformer.h"

namespace ra::module::tool::text_transform {

class UpperTransformer : public TextTransformer {
public:
    static CommandBrief Brief();

public:
    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    void Transform(std::wstring& text) override;
};

}