#pragma once

#include <string_view>
#include "help/markdown/element/element.h"

namespace ra::help {

std::shared_ptr<markdown::element::Element> LoadBuiltInHelpContent(std::wstring_view command);

}