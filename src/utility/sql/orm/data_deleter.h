#pragma once

#include "utility/sql/orm/primary_key_traits.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataDeleter {
private:
    using TableType = typename E::TableType;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    template<typename T = TableType>
    static void Delete(
        Database& db,
        const std::enable_if_t<HasPrimaryKeyV<T>, typename T::PrimaryKeyType::ValueType>& pk) {

        static auto sql = std::format("delete from {} where {}",
            Table().GetName(),
            MakeKeyEquation(Table().PrimaryKey));

        auto statement = db.PrepareStatement(sql);
        TableType::PrimaryKeyType::BindValueToStatement(statement, 1, pk);

        statement.Step();
    }
};

}