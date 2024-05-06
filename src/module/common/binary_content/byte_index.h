#pragma once

#include <cstddef>
#include <zaf/base/error/contract_error.h>
#include <zaf/base/relation_operator.h>
#include "module/common/binary_content/binary_content_common.h"

namespace ra::mod {

class ByteIndex {
public:
    ByteIndex() = default;

    ByteIndex(std::size_t line, std::size_t index_in_line) :
        line_(line), index_in_line_(index_in_line) {
    }

    std::size_t Line() const {
        return line_;
    }

    std::size_t IndexInLine() const {
        return index_in_line_;
    }

    std::size_t IndexInContent() const {
        return line_ * BytesPerLine + index_in_line_;
    }

private:
    std::size_t line_{};
    std::size_t index_in_line_{};
};


inline bool operator<(const ByteIndex& index1, const ByteIndex& index2) {
    return index1.IndexInContent() < index2.IndexInContent();
}

inline bool operator==(const ByteIndex& index1, const ByteIndex& index2) {
    return index1.IndexInContent() == index2.IndexInContent();
}

ZAF_DEFINE_RELATION_OPERATORS(ByteIndex);


class ByteIndexRange {
public:
    ByteIndexRange() = default;

    ByteIndexRange(const ByteIndex& first, const ByteIndex& last) : first_(first), last_(last) {
        ZAF_EXPECT(first <= last);
    }

    const ByteIndex& First() const {
        return first_;
    }

    const ByteIndex& Last() const {
        return last_;
    }

private:
    ByteIndex first_;
    ByteIndex last_;
};

}