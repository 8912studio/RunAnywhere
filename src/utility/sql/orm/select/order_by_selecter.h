#pragma once

#include "utility/sql/orm/select/selecter.h"
#include "utility/sql/orm/select/decorative_selecter_core.h"
#include "utility/sql/orm/select/limit_selecter.h"
#include "utility/sql/orm/select/ordering_column.h"

namespace ra::utility::sql {

template<typename InnerCore, typename... Columns>
class OrderBySelecter : public Selecter<OrderBySelecter<InnerCore, Columns...>> {
private:
    friend class Selecter<OrderBySelecter<InnerCore, Columns...>>;

    using OrderingColumnsType = decltype(MakeOrderingColumns((std::declval<Columns>())...));

    class Core : public DecorativeSelecterCore<InnerCore> {
    public:
        Core(InnerCore&& inner_core, OrderingColumnsType&& ordering_columns) :
            DecorativeSelecterCore<InnerCore>(std::move(inner_core)),
            ordering_columns_(std::move(ordering_columns)) {

        }

        std::string BuildSQL() const {

            static const std::string sql = [this]() {

                auto sql = this->GetInnerCore().BuildSQL();
                sql += " order by ";

                std::size_t index{};
                auto append = [&index, &sql](const auto& ordering_column) {
                    if (index != 0) {
                        sql += ',';
                    }
                    sql += ordering_column.BuildSQL();
                    ++index;
                };

                std::apply(
                    [&append](const auto&... columns) {
                        (append(columns), ...);
                    },
                    ordering_columns_);

                return sql;
            }();
            return sql;
        }

    private:
        OrderingColumnsType ordering_columns_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    OrderBySelecter(InnerCore inner_core, const Columns&... columns) :
        core_(std::move(inner_core), MakeOrderingColumns(columns...)) {

    }

    LimitSelecter<Core> Limit(std::size_t limit) const {
        return LimitSelecter<Core>(core_, limit);
    }
};

}