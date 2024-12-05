#pragma once

#include <format>
#include "utility/sql/database.h"
#include "utility/sql/orm/base_query.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/limit_query.h"
#include "utility/sql/orm/order_by_query.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class SelectQuery : public BaseQuery<SelectQuery<E, Columns...>> {
private:
    friend class BaseQuery<SelectQuery<E, Columns...>>;

    using CompositeColumnType = CompositeColumn<E, Columns...>;

    class Core {
    public:
        using EntityType = E;
        using ResultElementType = typename CompositeColumnType::ValueType;

    public:
        Core(Database& db, const Columns&... columns) : db_(db), composite_column_(columns...) {

        }

        std::string BuildSQL() const {
            return std::format(
                "select {} from {}",
                JoinColumnNames(composite_column_.GetAbstractColumns()),
                E::TableType::GetInstance().GetName());
        }

        Statement PrepareStatement(std::string_view sql) const {
            return db_.PrepareStatement(sql);
        }

        int BindParameters(Statement& statement, int begin_index) const {
            return begin_index;
        }

        ResultElementType GetElement(const Statement& statement) const {
            return CompositeColumnType::GetValueFromStatement(statement, 0);
        }

    private:
        Database& db_;
        CompositeColumnType composite_column_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    SelectQuery(Database& db, const Columns&... columns) : core_(db, columns...) {

    }

    template<typename... OrderColumns>
    auto OrderBy(const OrderColumns&... columns) const {
        return OrderByQuery<Core, OrderColumns...>(core_, columns...);
    }

    auto Limit(std::size_t limit) const {
        return LimitQuery<Core>(core_, limit);
    }
};

}