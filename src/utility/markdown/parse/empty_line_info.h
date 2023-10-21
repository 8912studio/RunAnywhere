#pragma once

namespace ra::utility::markdown::parse {

struct EmptyLineInfo {
    bool has_middle_empty_line{};
    bool has_trailing_empty_line{};
};

}