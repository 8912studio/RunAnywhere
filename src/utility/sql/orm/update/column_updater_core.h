#pragma once

#include <zaf/base/string/join.h>
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/expression/assignment.h"
#include "utility/sql/orm/update/primitive_updater_core.h"

namespace ra::utility::sql {

template<typename E, typename... Assign>
class ColumnUpdaterCore : public PrimitiveUpdaterCore<ColumnUpdaterCore<E, Assign...>> {
public:
    using EntityType = E;

public:
    ColumnUpdaterCore(Database& db, std::tuple<Assign...> assignments) :
        PrimitiveUpdaterCore<ColumnUpdaterCore<E, Assign...>>(db),
        assignments_(std::move(assignments)) {

    }

    int BindParameters(Statement& statement, int begin_index) const {

        int index = begin_index;
        auto bind = [&statement , &index](const auto& assignment) {
            index = assignment.BindParameters(statement, index);
        };

        std::apply(
            [&bind](const auto&... values) {
                (bind(values), ...);
            },
            assignments_);

        return index;
    }

private:
    friend class PrimitiveUpdaterCore<ColumnUpdaterCore<E, Assign...>>;

    std::string BuildAssignmentSQL() const {

        std::string result;
        std::size_t index = 0;
        auto append = [&result, &index](const auto& assignment) {
            if (index != 0) {
                result += ',';
            }
            ++index;
            result += assignment.BuildSQL();
        };

        std::apply(
            [&append](const auto&... values) {
                (append(values), ...);
            },
            assignments_);

        return result;
    }

    std::tuple<Assign...> assignments_;
};

}