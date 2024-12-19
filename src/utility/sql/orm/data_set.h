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
#include "utility/sql/orm/data_updater.h"
#include "utility/sql/orm/delete/primitive_deleter.h"
#include "utility/sql/orm/select/entity_selecter_core.h"
#include "utility/sql/orm/select/primitive_selecter.h"
#include "utility/sql/orm/table_initializer.h"
#include "utility/sql/orm/table.h"
#include "utility/sql/orm/update/column_updater_core.h"
#include "utility/sql/orm/update/entity_updater_core.h"
#include "utility/sql/orm/update/primitive_updater.h"

namespace ra::utility::sql {

template<typename E>
class DataSet : zaf::NonCopyableNonMovable {
private:
    using TableType = typename E::TableType;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    explicit DataSet(Database& db) : db_(DBWrapper{ db }) {

    }


    [[nodiscard]]
    auto BeginSelect() {
        using Core = EntitySelecterCore<E>;
        return PrimitiveSelecter<Core>(Core{ db_.Get() });
    }


    template<typename... Columns>
    [[nodiscard]]
    auto BeginSelect(const Columns&... columns) {
        using Core = ColumnSelecterCore<E, Columns...>;
        return PrimitiveSelecter<Core>(Core{ db_.Get(), columns... });
    }


    std::vector<E> SelectAll() {
        return BeginSelect().Execute();
    }


    template<typename T = TableType, typename = std::enable_if_t<HasPrimaryKeyV<T>>>
    std::optional<E> Select(const T::PrimaryKeyType::ValueType& primary_key) {
        auto result = BeginSelect().Where(Table().PrimaryKey == primary_key).Execute();
        if (!result.empty()) {
            return result.front();
        }
        return std::nullopt;
    }


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


    [[nodiscard]]
    auto BeginUpdate(const E& entity) {
        using Core = EntityUpdaterCore<E>;
        return PrimitiveUpdater<Core>(Core{ db_.Get(), entity });
    }


    template<typename... Assign>
    [[nodiscard]]
    auto BeginUpdate(Assign... assignments) {
        using Core = ColumnUpdaterCore<E, Assign...>;
        return PrimitiveUpdater<Core>{ 
            Core{ db_.Get(), std::make_tuple(std::move(assignments)...) } 
        };
    }


    template<typename T = TableType, typename = std::enable_if_t<HasPrimaryKeyV<T>>>
    void Update(const E& entity) {
        DataUpdater<E>::Update(db_.Get(), entity);
    }


    [[nodiscard]]
    auto BeginDelete() {
        using Deleter = PrimitiveDeleter<E>;
        return Deleter{ db_.Get() };
    }


    template<typename T = TableType>
    std::size_t Delete(
        const std::enable_if_t<
            HasPrimaryKeyV<T>, 
            typename T::PrimaryKeyType::ValueType
        >& primary_key) {

        return BeginDelete().Where(T::GetInstance().PrimaryKey == primary_key).Execute();
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