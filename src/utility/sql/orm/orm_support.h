#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/table.h"
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/expression/expression_support.h"
#include "utility/sql/orm/index.h"
#include "utility/sql/orm/macro_utility.h"
#include "utility/sql/orm/primary_key.h"
#include "utility/sql/orm/select/ordering_column.h"
#include "utility/sql/orm/value_type/nullable_value_type.h"
#include "utility/sql/orm/value_type/primitive_value_type.h"

#define SQL_ENTITY class TableType;

#define SQL_TABLE_BEGIN(TABLE_NAME, ENTITY_CLASS) \
class ENTITY_CLASS::TableType : public ra::utility::sql::AbstractTable { \
public: \
    using EntityType = ENTITY_CLASS; \
    static const TableType& GetInstance() { \
        static TableType instance; \
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
    ra::utility::sql::AbstractIndexesView GetAbstractIndexes() const noexcept override { \
        return indexes_; \
    } \
private: \
    TableType() = default; \
    std::vector<const ra::utility::sql::Column<EntityType>*> fields_; \
    std::vector<const ra::utility::sql::AbstractIndex*> indexes_;


#define SQL_COLUMN(COLUMN_NAME, CLASS_FIELD) \
public: \
    class COLUMN_NAME##Type : public ra::utility::sql::Column<EntityType> { \
    private: \
        using ThisType = COLUMN_NAME##Type; \
    public: \
        using ValueType = decltype(((EntityType*)nullptr)->CLASS_FIELD); \
        using ValueTypeTraits = ra::utility::sql::ValueTypeTraits<ValueType>; \
        using Column::Column; \
        std::string_view GetName() const noexcept override { \
            return #COLUMN_NAME; \
        } \
        ra::utility::sql::DataType GetDataType() const noexcept override { \
            return ValueTypeTraits::DataType; \
        } \
        bool IsNullable() const noexcept override { \
            return ValueTypeTraits::IsNullable; \
        } \
        void BindValueToStatement( \
            ra::utility::sql::Statement& statement, \
            int parameter_index, \
            const EntityType& entity) const override { \
            ValueTypeTraits::BindValueToStatement( \
                statement, \
                parameter_index, \
                entity.CLASS_FIELD); \
        } \
        void GetValueFromStatement( \
            const ra::utility::sql::Statement& statement, \
            int column_index, \
            EntityType& entity) const override { \
            entity.CLASS_FIELD = ValueTypeTraits::GetValueFromStatement(statement, column_index); \
        } \
        auto Asc() const { \
            return ra::utility::sql::OrderingColumn<ThisType, ra::utility::sql::AscOrder>{ \
                *this \
            }; \
        } \
        auto Desc() const { \
            return ra::utility::sql::OrderingColumn<ThisType, ra::utility::sql::DescOrder>{ \
                *this \
            }; \
        } \
        SQL_EXPRESSION_OPERATORS(ThisType, ValueType) \
    }; \
    COLUMN_NAME##Type COLUMN_NAME{ fields_ };


#define __SQL_DEFINE_AUTOINCREMENT \
struct AutoincrementTag {}; \
bool IsAutoincrement() const noexcept override { \
    return true; \
}

#define __SQL_DEFINE_PRIMARY_KEY(AUTO_INC, ...) \
private: \
    using BasePrimaryKeyType = \
        decltype(ra::utility::sql::DeducePrimaryKeyType<EntityType>(__VA_ARGS__)); \
public: \
    class PrimaryKeyType : public BasePrimaryKeyType { \
    public: \
        using BasePrimaryKeyType::BasePrimaryKeyType; \
        AUTO_INC \
        SQL_EXPRESSION_OPERATORS(PrimaryKeyType, PrimaryKeyType::ValueType) \
    }; \
    PrimaryKeyType PrimaryKey{ __VA_ARGS__ }; \
    const ra::utility::sql::AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }

#define SQL_PRIMARY_KEY(...) __SQL_DEFINE_PRIMARY_KEY(, __VA_ARGS__)

#define SQL_PRIMARY_KEY_AUTOINCREMENT(COLUMN_NAME) \
__SQL_DEFINE_PRIMARY_KEY(__SQL_DEFINE_AUTOINCREMENT, COLUMN_NAME)


#define SQL_INDEX(...) \
public: \
    using SQL_UTILITY_JOIN(__VA_ARGS__)##IndexType = \
        decltype(ra::utility::sql::MakeIndex<EntityType>(indexes_, __VA_ARGS__)); \
    SQL_UTILITY_JOIN(__VA_ARGS__)##IndexType SQL_UTILITY_JOIN(__VA_ARGS__)##Index = \
        ra::utility::sql::MakeIndex<EntityType>(indexes_, __VA_ARGS__);

#define SQL_TABLE_END };
