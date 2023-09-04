#pragma once

namespace ra::help::markdown {

enum class StyleType {
    Normal,
    Header1,
    Header2,
    Header3,
    Bold,
    InlineCode,
    CodeBlock,
};


class Style {
public:
    StyleType type{ StyleType::Normal };

    friend auto operator<=>(const Style&, const Style&) = default;
};

}