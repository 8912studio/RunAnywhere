#pragma once

#include <format>
#include <mutex>
#include <vector>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/container/utility/copy.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_inserter.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/table_initializer.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataSet : zaf::NonCopyableNonMovable {
private:
    using TableType = Table<E>;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    explicit DataSet(Database& db) : db_(DBWrapper{ db }) {

    }


    std::vector<E> SelectAll() {

        static auto sql = std::format(
            "select {} from {}", 
            JoinColumnNames(Table().GetAbstractColumns()), 
            Table().GetName());

        std::vector<E> result;

        auto statement = db_.Get().PrepareStatement(sql);
        while (statement.Step()) {

            E entity{};
            GetEntityValuesFromStatement(statement, Table().GetColumns(), entity);

            result.push_back(std::move(entity));
        }

        return result;
    }

    /*
    std::optional<E> Select(const TableType::PrimaryKeyType::ValueType& primary_key) {

        static auto sql = std::format("select {} from {} where {}",
            JoinColumnNames(Table().GetAbstractColumns()),
            Table().GetName(),
            MakeKeyEquation(Table().PrimaryKey));

        auto statement = db_.Get().PrepareStatement(sql);
        TableType::PrimaryKeyType::BindValue(statement, 1, primary_key);

        if (statement.Step()) {

            E entity{};
            GetEntityValuesFromStatement(statement, Table().GetColumns(), entity);
            return std::move(entity);
        }

        return std::nullopt;
    }
    */


    void Insert(const E& entity) {
        DataInserter<E>::Insert(db_.Get(), entity);
    }


    template<typename T = TableType>
    std::enable_if_t<HasAutoincrementPrimaryKeyV<T>, typename T::PrimaryKeyType::ValueType> 
        InsertWithAutoincrement(const E& entity) {
        return DataInserter<E>::InsertWithAutoincrement(db_.Get(), entity);
    }


    void Replace(const E& entity) {
        DataInserter<E>::Replace(entity, db_.Get());
    }


    void Update(const E& entity) {

        const auto& primary_key_fields = Table().PrimaryKey.Fields();
        auto non_primary_key_fields = zaf::CopyIf(Table().GetColumns(), [&](auto field) {
            return !zaf::Contain(primary_key_fields, field);
        });

        static auto sql = [&non_primary_key_fields]() {
        
            auto sql = std::format("update {} set {} where {}",
                Table().GetName(),
                zaf::JoinAsString(non_primary_key_fields, ",", [](auto field) {
                    return std::format("{}=?", field->GetName());
                }),
                MakeKeyEquation(Table().PrimaryKey));

            return sql;
        }();

        auto statement = db_.Get().PrepareStatement(sql);
        BindEntityValuesToStatement<E>(statement, 1, non_primary_key_fields, entity);

        BindEntityValuesToStatement<E>(
            statement, 
            static_cast<int>(1 + non_primary_key_fields.size()), 
            primary_key_fields, 
            entity);

        statement.Step();
    }

    /*
    void Delete(const TableType::PrimaryKeyType::ValueType& primary_key) {

        static auto sql = std::format("delete from {} where {}",
            Table().GetName(),
            MakeKeyEquation(Table().PrimaryKey));

        auto statement = db_.Get().PrepareStatement(sql);
        TableType::PrimaryKeyType::BindValue(statement, 1, primary_key);

        statement.Step();
    }
    */

private:
    static void GetEntityValuesFromStatement(
        const Statement& statement, 
        std::span<const Column<E>* const> fields,
        E& entity) {

        for (auto index : zaf::Range{ 0, fields.size() }) {
            fields[index]->GetValueFromStatement(statement, static_cast<int>(index), entity);
        }
    }

private:
    class DBWrapper : zaf::NonCopyableNonMovable {
    public:
        explicit DBWrapper(Database& db) noexcept : db_(db) {

        }

        Database& Get() {
            std::call_once(init_once_flag_, [this]() {
                TableInitializer::Initialize(Table(), db_);
            });
            return db_;
        }

    private:
        Database& db_;
        std::once_flag init_once_flag_;
    };

private:
    DBWrapper db_;
};

}