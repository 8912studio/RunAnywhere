#include "test/fault_injection/probability.h"

namespace ra::test {
namespace {

std::once_flag g_srand_once_flag;

}

ZAF_OBJECT_IMPL(Probability);

void Probability::ReviseValue() {

    if (value_ < MinValue) {
        value_ = MinValue;
    }
    else if (value_ > MaxValue) {
        value_ = MaxValue;
    }
}


bool Probability::Roll() const {

    if (value_ == MinValue) {
        return false;
    }

    if (value_ == MaxValue) {
        return true;
    }

    std::call_once(g_srand_once_flag, []() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    });

    int rand_value = std::rand();
    auto percent = float(rand_value) / float(RAND_MAX);
    return (int(percent * 10) <= value_);
}

}