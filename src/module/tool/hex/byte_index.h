#pragma once

#include <cstddef>
#include "module/tool/hex/paint_common.h"

namespace ra::module::tool::hex {

class ByteIndex {
public:
    ByteIndex() = default;

    ByteIndex(std::size_t line_index, std::size_t index_in_line) :
        line_index_(line_index), index_in_line_(index_in_line) {
    }

    std::size_t Line() const {
        return line_index_;
    }

    std::size_t IndexInLine() const {
        return index_in_line_;
    }

    std::size_t IndexInContent() const {
        return line_index_ * BytesPerLine + index_in_line_;
    }

private:
    std::size_t line_index_{};
    std::size_t index_in_line_{};
};


inline bool operator==(const ByteIndex& index1, const ByteIndex& index2) {
    return
        (index1.Line() == index2.Line()) &&
        (index1.IndexInLine() == index2.IndexInLine());
}

}