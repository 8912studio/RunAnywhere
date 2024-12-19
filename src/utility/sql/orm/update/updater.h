#pragma once

#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename U>
class Updater {
public:
    std::size_t Execute() const {

        const auto& updater = static_cast<const U&>(*this);
        const auto& core = updater.GetCore();

        std::string sql = core.BuildSQL();

        auto& db = core.GetDB();
        auto statement = db.PrepareStatement(sql);
        core.BindParameters(statement, 1);
        statement.Step();

        return db.LastChanges();
    }
};

}