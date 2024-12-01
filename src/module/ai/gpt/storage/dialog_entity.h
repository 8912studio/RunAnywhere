#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include "utility/sql/orm/orm_support.h"

namespace ra::mod::ai::gpt {

struct DialogEntity {

    SQL_ENTITY;

    std::uint64_t id{};
    std::time_t create_time{};
    std::time_t update_time{};
    std::string subject;
};

SQL_TABLE_BEGIN(Dialog, DialogEntity)
SQL_COLUMN(ID, id)
SQL_COLUMN(CreateTime, create_time)
SQL_COLUMN(UpdateTime, update_time)
SQL_COLUMN(Subject, subject)
SQL_PRIMARY_KEY_AUTOINCREMENT(ID)
SQL_TABLE_END

}