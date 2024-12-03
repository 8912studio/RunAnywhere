#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

struct RoundEntity {

    SQL_ENTITY;

    std::uint64_t id{};
    std::uint64_t dialog_id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string question;
    std::string response;
};

SQL_TABLE_BEGIN(Round, RoundEntity)
SQL_COLUMN(ID, id)
SQL_COLUMN(DialogID, dialog_id)
SQL_COLUMN(CreateTime, create_time)
SQL_COLUMN(UpdateTime, update_time)
SQL_COLUMN(Question, question)
SQL_COLUMN(Response, response)
SQL_PRIMARY_KEY_AUTOINCREMENT(ID)
SQL_INDEX(DialogID)
SQL_TABLE_END

}