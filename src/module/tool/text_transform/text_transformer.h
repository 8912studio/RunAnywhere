#pragma once

#include <zaf/base/non_copyable.h>
#include "help/content/content.h"
#include "module/command_brief.h"

namespace ra::module::tool::text_transform {

class TextTransformer : public zaf::NonCopyableNonMovable {
public:
    TextTransformer() = default;
    virtual ~TextTransformer() = default;

    virtual CommandBrief GetBrief() = 0;
    virtual help::content::Content GetHelpContent() = 0;
    virtual void Transform(std::wstring& text) = 0;
};

}