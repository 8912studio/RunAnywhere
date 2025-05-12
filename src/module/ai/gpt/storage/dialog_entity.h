#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <sqt/orm/table_definition.h>
#include <sqt/orm/table_mapping.h>

namespace ra::mod::ai::gpt {

struct DialogEntity {
    std::uint64_t id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string subject;
};

SQT_TABLE_BEGIN(Dialog, DialogEntity)
SQT_COLUMN_FIELD(ID, id)
SQT_COLUMN_FIELD(CreateTime, create_time)
SQT_COLUMN_FIELD(UpdateTime, update_time)
SQT_COLUMN_FIELD(Subject, subject)
SQT_PRIMARY_KEY_AUTO_INC(ID)
SQT_TABLE_END

}

SQT_REGISTER(ra::mod::ai::gpt::Dialog)