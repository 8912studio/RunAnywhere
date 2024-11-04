#pragma once

#include <cstdint>
#include <zaf/object/object.h>

namespace ra::test {

class Probability : public zaf::Object {
public:
    ZAF_OBJECT;

public:
    Probability() = default;

    Probability(std::size_t value) noexcept : value_(value) {

    }

    Probability& operator=(std::size_t value) noexcept {
        value_ = value;
        return *this;
    }

    operator std::size_t() const noexcept {
        return value_;
    }

private:
    std::size_t value_{};
};

ZAF_OBJECT_BEGIN(Probability);
ZAF_OBJECT_END;

}