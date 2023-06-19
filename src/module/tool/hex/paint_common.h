#pragma once

#include <cstddef>
#include <string>
#include <zaf/graphic/color.h>
#include <zaf/graphic/text/text_layout.h>

namespace ra::mod::tool::hex {

constexpr float LineHeight = 24;
constexpr float LineHeaderWidth = 32;
constexpr float LineHeaderMargin = 6;
constexpr float ByteWidth = 22;
constexpr float ByteGapWidth = 8;
constexpr std::size_t BytesPerLine = 16;
constexpr float CharacterAreaGapWidth = 24;
constexpr float CharacterWidth = 10;

inline constexpr float ByteHexAreaX() {
    return LineHeaderWidth + LineHeaderMargin;
}

inline constexpr float ByteHexFirstPartEndX() {
    return ByteHexAreaX() + (BytesPerLine / 2) * ByteWidth;
}

inline constexpr float ByteHexSecondPartBeginX() {
    return ByteHexFirstPartEndX() + ByteGapWidth;
}

inline constexpr float ByteHexSecondPartEndX() {
    return ByteHexSecondPartBeginX() + (BytesPerLine / 2) * ByteWidth;
}

inline constexpr float ByteCharacterAreaX() {
    return ByteHexAreaX() + BytesPerLine * ByteWidth + ByteGapWidth + CharacterAreaGapWidth;
}

inline constexpr float ByteCharacterAreaEndX() {
    return ByteCharacterAreaX() + BytesPerLine * CharacterWidth;
}


inline zaf::Color HeaderBackgroundColor() {
    return zaf::Color::FromRGB(0xF0F0F0);
}


inline zaf::Color HeaderTextColor() {
    return zaf::Color::FromRGB(0xAAAAAA);
}


zaf::TextLayout CreateCommonTextLayout(const std::wstring& text, float layout_width);

std::wstring ToHexString(std::size_t value, std::size_t min_length);

}