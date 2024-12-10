#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/table.h"
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/column_value_traits.h"
#include "utility/sql/orm/expression/expression.h"
#include "utility/sql/orm/field_traits.h"
#include "utility/sql/orm/index.h"
#include "utility/sql/orm/macro_utility.h"
#include "utility/sql/orm/primary_key.h"

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
        using ValueTraits = ra::utility::sql::ColumnValueTraits<ValueType>; \
        using Column::Column; \
        std::string_view GetName() const noexcept override { \
            return #COLUMN_NAME; \
        } \
        ra::utility::sql::DataType GetDataType() const noexcept override { \
            return ValueTraits::DataType; \
        } \
        bool IsNullable() const noexcept override { \
            return ValueTraits::IsNullable; \
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
        friend auto operator==(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::Equal); \
        } \
        friend auto operator==(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::Equal); \
        } \
        friend auto operator!=(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::NotEqual); \
        } \
        friend auto operator!=(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::NotEqual); \
        } \
        friend auto operator<(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::Less); \
        } \
        friend auto operator<(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::Less); \
        } \
        friend auto operator<=(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::LessEqual); \
        } \
        friend auto operator<=(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::LessEqual); \
        } \
        friend auto operator>(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::Greater); \
        } \
        friend auto operator>(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::Greater); \
        } \
        friend auto operator>=(const ThisType& column, const ValueType& value) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                column, value, ra::utility::sql::Operator::GreaterEqual); \
        } \
        friend auto operator>=(const ValueType& value, const ThisType& column) { \
            return ra::utility::sql::MakeExpressionWithOperands( \
                value, column, ra::utility::sql::Operator::GreaterEqual); \
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
    class PrimaryKeyType : public ra::utility::sql::PrimaryKey<EntityType, COLUMN_NAME##Type> { \
    public: \
        struct AutoincrementTag {}; \
        using PrimaryKey::PrimaryKey; \
        bool IsAutoincrement() const noexcept override { \
            return true; \
        } \
    }; \
    PrimaryKeyType PrimaryKey{ COLUMN_NAME }; \
    const ra::utility::sql::AbstractPrimaryKey* GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }


#define SQL_INDEX(...) \
public: \
    using SQL_UTILITY_JOIN(__VA_ARGS__)##IndexType = \
        decltype(ra::utility::sql::MakeIndex<EntityType>(indexes_, __VA_ARGS__)); \
    SQL_UTILITY_JOIN(__VA_ARGS__)##IndexType SQL_UTILITY_JOIN(__VA_ARGS__)##Index = \
        ra::utility::sql::MakeIndex<EntityType>(indexes_, __VA_ARGS__);

#define SQL_TABLE_END };
