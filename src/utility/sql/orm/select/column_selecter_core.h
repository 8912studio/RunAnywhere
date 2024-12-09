#pragma once

#include "utility/sql/orm/select/primitive_selecter_core.h"
#include "utility/sql/orm/composite_column.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class ColumnSelecterCore : public PrimitiveSelecterCore<ColumnSelecterCore<E, Columns...>> {
public:
    using CompositeColumnType = CompositeColumn<E, Columns...>;

    using EntityType = E;
    using ResultElementType = typename CompositeColumnType::ValueType;

public:
    ColumnSelecterCore(Database& db, const Columns&... columns) : 
        PrimitiveSelecterCore<ColumnSelecterCore<E, Columns...>>(db),
        composite_column_(columns...) {

    }

    ResultElementType GetElement(const Statement& statement) const {
        return CompositeColumnType::GetValueFromStatement(statement, 0);
    }

private:
    friend class PrimitiveSelecterCore<ColumnSelecterCore<E, Columns...>>;

    AbstractColumnsView GetAbstractColumns() const {
        return composite_column_.GetAbstractColumns();
    }

    CompositeColumnType composite_column_;
};

}