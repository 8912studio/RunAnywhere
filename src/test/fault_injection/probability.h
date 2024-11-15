#pragma once

#include <cstdint>
#include <zaf/object/object.h>

namespace ra::test {

class Probability : public zaf::Object {
public:
    ZAF_OBJECT;

    static constexpr int MinValue = 0;
    static constexpr int MaxValue = 10;

public:
    Probability() = default;

    explicit Probability(int value) noexcept : value_(value) {
        ReviseValue();
    }

    Probability& operator=(int value) noexcept {
        value_ = value;
        ReviseValue();
        return *this;
    }

    explicit operator int() const noexcept {
        return value_;
    }

    bool Roll() const;

private:
    void ReviseValue();

private:
    int value_{};
};

ZAF_OBJECT_BEGIN(Probability);
ZAF_OBJECT_END;

}