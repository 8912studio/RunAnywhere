#pragma once

#include "module/tool/text_transform/text_transformer.h"

namespace ra::mod::tool::text_transform {

class UpperTransformer : public TextTransformer {
public:
    static std::wstring Keyword();

public:
    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;
    void Transform(std::wstring& text) override;
};

}