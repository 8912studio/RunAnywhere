#pragma once

#include <cstdint>
#include <ctime>
#include <string>

namespace ra::mod::ai::gpt {

using DialogID = std::uint64_t;

struct DialogEntity {

    DialogID id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string subject;
};

}