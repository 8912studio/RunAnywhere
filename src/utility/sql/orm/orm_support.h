#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/entity_meta.h"
#include "utility/sql/orm/field.h"
#include "utility/sql/orm/primary_key.h"
#include "utility/sql/orm/field_traits.h"
#include "utility/sql/orm/field_value_traits.h"

#define SQL_ENTITY_BEGIN(CLASS) \
template<> \
class sql__EntityMeta<CLASS> : public ra::utility::sql::AbstractEntityMeta { \
public: \
    using EntityType = CLASS; \
    static const sql__EntityMeta& Instance() { \
        static sql__EntityMeta instance; \
        return instance; \
    } \
    std::string_view GetName() const noexcept override { return #CLASS; } \
    ra::utility::sql::AbstractFieldsView GetAllAbstractFields() const noexcept override { \
        return { \
            reinterpret_cast<const ra::utility::sql::AbstractField* const*>(fields_.data()), \
            fields_.size() \
        };\
    } \
    ra::utility::sql::FieldsView<EntityType> GetAllFields() const noexcept { \
        return fields_; \
    } \
private: \
    sql__EntityMeta() = default; \
    std::vector<const ra::utility::sql::Field<EntityType>*> fields_;


#define SQL_ENTITY_FIELD(NAME, FIELD) \
public: \
    class NAME##Type : public ra::utility::sql::Field<EntityType> { \
    public: \
        using ValueType = decltype(((EntityType*)nullptr)->FIELD); \
        using Field::Field; \
        std::string_view Name() const override { \
            return #NAME; \
        } \
        ra::utility::sql::DataType DataType() const override { \
            return ra::utility::sql::FieldValueTraits<ValueType>::DataType; \
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


#define SQL_ENTITY_PRIMARY_KEY(...) \
public:\
    using PrimaryKeyType = decltype(ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__)); \
    PrimaryKeyType PrimaryKey = ra::utility::sql::MakePrimaryKey<EntityType>(__VA_ARGS__); \
    const ra::utility::sql::AbstractPrimaryKey* \
        GetAbstractPrimaryKey() const noexcept override { \
        return &PrimaryKey; \
    }


#define SQL_ENTITY_END };