#include "utility/clipboard.h"
#include <zaf/base/clipboard.h>
#include <zaf/base/error/error.h>

namespace ra::utility {

void SetStringToClipboard(const std::wstring& string) {

    try {
        zaf::Clipboard::SetText(string);
    }
    catch (const zaf::Error&) {
        //Ignore errors.
    }
}

}