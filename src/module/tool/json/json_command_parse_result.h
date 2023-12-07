#pragma once

#include <string>
#include "utility/markdown/render/styled_text.h"

namespace ra::mod::tool::json {

class JSONCommandParseResult {
public:
    utility::markdown::render::StyledText styled_text;
};

}