#pragma once

#include <string>
#include <zaf/base/non_copyable.h>

namespace ra::help::markdown {

class PlainStringBuilder : zaf::NonCopyable {
public:
    void SetIsFirstPart(bool is_first_part) {
        is_first_part_ = is_first_part;
    }

    void Append(wchar_t ch);

    std::wstring Build(bool is_last_part);

private:
    void RemoveTailingSpaces();

private:
    std::wstring string_;
    bool is_first_part_{};
};

}