#pragma once

#include <string_view>
#include <zaf/base/non_copyable.h>

namespace ra::utility::markdown::render {

class SelectedTextBuilder : zaf::NonCopyableNonMovable {
public:
    class RegionGuard : zaf::NonCopyable {
    public:
        explicit RegionGuard(SelectedTextBuilder& builder) : 
            builder_(builder),
            initial_length_(builder.text_.length()) {

        }

        ~RegionGuard() {
            if (builder_.text_.length() != initial_length_) {
                builder_.is_region_end_ = true;
            }
        }

    private:
        SelectedTextBuilder& builder_;
        std::size_t initial_length_{};
    };

public:
    [[nodiscard]]
    RegionGuard BeginRegion() {
        return RegionGuard{ *this };
    }

    void Append(std::wstring_view text) {

        if (is_region_end_) {
            is_region_end_ = false;
            text_.append(L"\r\n\r\n");
        }

        text_.append(text);
    }

    const std::wstring& Text() const {
        return text_;
    }

private:
    std::wstring text_;
    bool is_region_end_{};
};

}