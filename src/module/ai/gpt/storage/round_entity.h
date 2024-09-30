#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

struct RoundEntity {

    std::uint64_t id{};
    std::uint64_t dialog_id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string question;
    std::string response;
};

}