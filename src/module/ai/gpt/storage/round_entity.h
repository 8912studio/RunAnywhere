#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <sqt/orm/table_definition.h>
#include <sqt/orm/table_mapping.h>
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

SQT_TABLE_BEGIN(Round, RoundEntity)
SQT_COLUMN_FIELD(ID, id)
SQT_COLUMN_FIELD(DialogID, dialog_id)
SQT_COLUMN_FIELD(CreateTime, create_time)
SQT_COLUMN_FIELD(UpdateTime, update_time)
SQT_COLUMN_FIELD(Question, question)
SQT_COLUMN_FIELD(Response, response)
SQT_PRIMARY_KEY_AUTO_INC(ID)
SQT_INDEX_2(Index_Round_DialogID, Index_DialogID, DialogID)
SQT_TABLE_END

}

SQT_REGISTER(ra::mod::ai::gpt::Round)