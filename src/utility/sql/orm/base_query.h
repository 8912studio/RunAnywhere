#pragma once

#include <vector>
#include "utility/sql/database.h"

namespace ra::utility::sql {

template<typename Q>
class BaseQuery {
public:
    auto Execute() const {

        const auto& query = static_cast<const Q&>(*this);
        const auto& core = query.GetCore();

        std::string sql = core.BuildSQL();

        auto statement = core.PrepareStatement(sql);
        core.BindParameters(statement, 1);

        using CoreType = std::decay_t<decltype(core)>;
        std::vector<typename CoreType::ResultElementType> result;

        while (statement.Step()) {
            auto element = core.GetElement(statement);
            result.push_back(std::move(element));
        }

        return result;
    }
};

}