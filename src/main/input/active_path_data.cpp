#include "main/input/active_path_data.h"
#include <zaf/base/string/trim.h>
#include "utility/text_utility.h"

namespace ra::main::input {
namespace {

std::wstring NormalizeText(std::wstring text) {
    utility::RemoveMultipleLines(text);
    zaf::Trim(text);
    return text;
}

}

//Heading and tailing spaces are not allowed.
ActivePathData::ActivePathData(std::wstring text) : ArgumentData(NormalizeText(std::move(text))) {

}

}