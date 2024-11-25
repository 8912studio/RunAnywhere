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
    static const sql__Table& GetInstance() { \
        static sql__Table instance; \
        return instance; \
    } \
    std::string_view GetName() const noexcept override { return #TABLE_NAME; } \
    ra::utility::sql::AbstractColumnsView GetAbstractColumns() const noexcept override { \
        return { \
            reinterpret_cast<const ra::utility::sql::AbstractColumn* const*>(fields_.data()), \
            fields_.size() \
        };\
    } \
    ra::utility::sql::ColumnsView<EntityType> GetColumns() const noexcept { \
        return fields_; \
    } \
private: \
    sql__Table() = default; \
    std::vector<const ra::utility::sql::Column<EntityType>*> fields_;


#define SQL_COLUMN(COLUMN_NAME, CLASS_FIELD) \
public: \
    class COLUMN_NAME##Type : public ra::utility::sql::Column<EntityType> { \
    public: \
        using ValueType = decltype(((EntityType*)nullptr)->CLASS_FIELD); \
        using ValueTraits = ra::utility::sql::ColumnValueTraits<ValueType>; \
        using Column::Column; \
        std::string_view GetName() const noexcept override { \
            return #COLUMN_NAME; \
        } \
        ra::utility::sql::DataType GetDataType() const noexcept override { \
            return ValueTraits::DataType; \
        } \
        bool IsNotNull() const noexcept override { \
            return ValueTraits::IsNotNull; \
        } \
        void BindValueToStatement( \
            ra::utility::sql::Statement& statement, \
            int parameter_index, \
            const EntityType& entity) const override { \
            ValueTraits::BindValueToStatement(statement, parameter_index, entity.CLASS_FIELD); \
        } \
        void GetValueFromStatement( \
            const ra::utility::sql::Statement& statement, \
            int column_index, \
            EntityType& entity) const override { \
            entity.CLASS_FIELD = ra::utility::sql::GetValueFromStatement<ValueType>( \
                statement, \
                column_index); \
        } \
    }; \
    COLUMN_NAME##Type COLUMN_NAME{ fields_ };


#define SQL_PRIMARY_KEY(...) \
public:\
    using PrimaryKeyType = decltype(ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__)); \
    PrimaryKeyType PrimaryKey = ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__); \
    const ra::utility::sql::AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }


#define SQL_PRIMARY_KEY_AUTOINCREMENT(COLUMN_NAME) \
public: \
    using PrimaryKeyType = ra::utility::sql::PrimaryKey<EntityType, COLUMN_NAME##Type>; \
    PrimaryKeyType PrimaryKey{ COLUMN_NAME, true }; \
    const ra::utility::sql::AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }

#define SQL_TABLE_END };