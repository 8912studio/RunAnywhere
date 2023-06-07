#include "utility/clipboard.h"
#include <zaf/base/error/error.h>
#include <zaf/clipboard/clipboard.h>

namespace ra::utility {

void SetStringToClipboard(const std::wstring& string) {

    try {
        zaf::clipboard::Clipboard::SetText(string);
    }
    catch (const zaf::Error&) {
        //Ignore errors.
    }
}

}