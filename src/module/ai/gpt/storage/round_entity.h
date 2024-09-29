#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

using RoundPermanentID = std::uint64_t;
using RoundTransientID = std::uint64_t;

struct RoundID {

    RoundPermanentID permanent_id{};
    RoundTransientID transient_id{};

    friend bool operator==(const RoundID& id1, const RoundID& id2) {

        if (id1.permanent_id != 0 && id2.permanent_id != 0) {
            return id1.permanent_id == id2.permanent_id;
        }

        if (id1.permanent_id == 0 && id2.permanent_id == 0) {
            return id1.transient_id == id2.transient_id;
        }

        return false;
    }
};

struct RoundEntity {

    RoundID ID() const {
        return RoundID{ permanent_id, transient_id };
    }

    RoundPermanentID permanent_id{};
    RoundTransientID transient_id{};
    DialogID dialog_id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string question;
    std::string response;
};

}