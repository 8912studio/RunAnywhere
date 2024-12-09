#pragma once

#include <vector>
#include "utility/sql/database.h"

namespace ra::utility::sql {

/**
The base selecter class, uses CRTP idiom to define unified interfaces for selecters.

@tparam S
    A selected type which derives from `Selecter`.

@details
    `S` should have the following method:
    - const Core& GetCore() const;

    `Core` should have the following type alias:
    - EntityType
    - ResultElementType

    and should have the following methods:
    - std::string BuildSQL() const;
    - Statement PrepareStatement(std::string_view) const;
    - int BindParameters(Statement&, int) const;
    - ResultElementType GetElement(const Statement&) const;
*/
template<typename S>
class Selecter {
public:
    auto Execute() const {

        const auto& selecter = static_cast<const S&>(*this);
        const auto& core = selecter.GetCore();

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