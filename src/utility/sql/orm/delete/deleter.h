#pragma once

#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename D>
class Deleter {
public:
    void Execute() const {

        const auto& deleter = static_cast<const D&>(*this);
        const auto& core = deleter.GetCore();

        std::string sql = core.BuildSQL();
        
        auto statement = core.PrepareStatement(sql);
        core.BindParameters(statement, 1);
        statement.Step();
    }
};

}