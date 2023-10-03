#pragma once

#include <zaf/base/non_copyable.h>
#include "help/help_content.h"
#include "module/command_brief.h"

namespace ra::mod::tool::text_transform {

class TextTransformer : public zaf::NonCopyableNonMovable {
public:
    TextTransformer() = default;
    virtual ~TextTransformer() = default;

    virtual std::wstring GetKeyword() = 0;
    virtual help::HelpContent GetHelpContent() = 0;
    virtual void Transform(std::wstring& text) = 0;
};

}