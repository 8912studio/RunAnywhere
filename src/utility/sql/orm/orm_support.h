#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/table.h"
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/primary_key.h"
#include "utility/sql/orm/field_traits.h"
#include "utility/sql/orm/column_value_traits.h"

#define SQL_TABLE_BEGIN(TABLE_NAME, ENTITY_CLASS) \
template<> \
class sql__Table<ENTITY_CLASS> : public ra::utility::sql::AbstractTable { \
public: \
    using EntityType = ENTITY_CLASS; \
    static const sql__Table& Instance() { \
        static sql__Table instance; \
        return instance; \
    } \
    std::string_view GetName() const noexcept override { return #TABLE_NAME; } \
    ra::utility::sql::AbstractColumnsView GetAllAbstractColumns() const noexcept override { \
        return { \
            reinterpret_cast<const ra::utility::sql::AbstractColumn* const*>(fields_.data()), \
            fields_.size() \
        };\
    } \
    ra::utility::sql::ColumnsView<EntityType> GetAllFields() const noexcept { \
        return fields_; \
    } \
private: \
    sql__Table() = default; \
    std::vector<const ra::utility::sql::Column<EntityType>*> fields_;


#define SQL_COLUMN(NAME, FIELD) \
public: \
    class NAME##Type : public ra::utility::sql::Column<EntityType> { \
    public: \
        using ValueType = decltype(((EntityType*)nullptr)->FIELD); \
        using Column::Column; \
        std::string_view GetName() const noexcept override { \
            return #NAME; \
        } \
        ra::utility::sql::DataType GetDataType() const noexcept override { \
            return ra::utility::sql::ColumnValueTraits<ValueType>::DataType; \
        } \
        void BindValueToStatement( \
            ra::utility::sql::Statement& statement, \
            int parameter_index, \
            const EntityType& entity) const override { \
            statement.BindParameter(parameter_index, entity.FIELD); \
        } \
        void GetValueFromStatement( \
            const ra::utility::sql::Statement& statement, \
            int column_index, \
            EntityType& entity) const override { \
            entity.FIELD = ra::utility::sql::GetValueFromStatement<ValueType>( \
                statement, \
                column_index); \
        } \
    }; \
    NAME##Type NAME{ fields_ };


#define SQL_PRIMARY_KEY(...) \
public:\
    using PrimaryKeyType = decltype(ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__)); \
    PrimaryKeyType PrimaryKey = ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__); \
    const ra::utility::sql::AbstractPrimaryKey* \
        GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }


#define SQL_TABLE_END };