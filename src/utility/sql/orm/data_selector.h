#pragma once

#include <vector>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataSelector {
private:
    using TableType = typename E::TableType;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    static std::vector<E> SelectAll(Database& db) {

        static auto sql = std::format(
            "select {} from {}",
            JoinColumnNames(Table().GetAbstractColumns()),
            Table().GetName());

        std::vector<E> result;

        auto statement = db.PrepareStatement(sql);
        while (statement.Step()) {

            E entity{};
            GetEntityValuesFromStatement(statement, Table().GetColumns(), entity);

            result.push_back(std::move(entity));
        }

        return result;
    }

private:
    static void GetEntityValuesFromStatement(
        const Statement& statement,
        std::span<const Column<E>* const> fields,
        E& entity) {

        for (auto index : zaf::Range{ 0, fields.size() }) {
            fields[index]->GetValueFromStatement(statement, static_cast<int>(index), entity);
        }
    }
};

}