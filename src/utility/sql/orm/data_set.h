#pragma once

#include <format>
#include <vector>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/container/utility/copy.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/entity_meta.h"

namespace ra::utility::sql {

template<typename E>
class DataSet : zaf::NonCopyableNonMovable {
private:
    using MetaType = EntityMeta<E>;

    static const MetaType& Meta() {
        return MetaType::Instance();
    }

public:
    explicit DataSet(Database& database) : database_(database) {

    }


    std::vector<E> SelectAll() const {

        static auto sql = std::format(
            "select {} from {}", 
            JoinFieldNames(Meta().Fields()), 
            Meta().EntityName());

        std::vector<E> result;

        auto statement = database_.PrepareStatement(sql);
        while (statement.Step()) {

            E entity{};
            GetEntityValuesFromStatement(statement, Meta().Fields(), entity);

            result.push_back(std::move(entity));
        }

        return result;
    }


    std::optional<E> Select(const MetaType::PrimaryKeyType::ValueType& primary_key) const {

        const auto& primary_key_fields = Meta().PrimaryKey.Fields();

        static auto sql = std::format("select {} from {} where {}",
            JoinFieldNames(Meta().Fields()), 
            Meta().EntityName(),
            MakeKeyEquation(Meta().PrimaryKey));

        auto statement = database_.PrepareStatement(sql);
        MetaType::PrimaryKeyType::BindValue(statement, 1, primary_key);

        if (statement.Step()) {

            E entity{};
            GetEntityValuesFromStatement(statement, Meta().Fields(), entity);
            return std::move(entity);
        }

        return std::nullopt;
    }


    void Insert(const E& entity) {
        InsertOrReplace(entity, true);
    }


    void Replace(const E& entity) {
        InsertOrReplace(entity, false);
    }


    void Update(const E& entity) {

        const auto& primary_key_fields = Meta().PrimaryKey.Fields();
        auto non_primary_key_fields = zaf::CopyIf(Meta().Fields(), [&](auto field) {
            return !zaf::Contain(primary_key_fields, field);
        });

        static auto sql = [&non_primary_key_fields]() {
        
            auto sql = std::format("update {} set {} where {}",
                Meta().EntityName(),
                zaf::JoinAsString(non_primary_key_fields, ",", [](auto field) {
                    return std::format("{}=?", field->Name());
                }),
                MakeKeyEquation(Meta().PrimaryKey));

            return sql;
        }();

        auto statement = database_.PrepareStatement(sql);
        BindEntityValuesToStatement(statement, 1, non_primary_key_fields, entity);

        BindEntityValuesToStatement(
            statement, 
            static_cast<int>(1 + non_primary_key_fields.size()), 
            primary_key_fields, 
            entity);

        statement.Step();
    }

    
    void Delete(const MetaType::PrimaryKeyType::ValueType& primary_key) {

        static auto sql = std::format("delete from {} where {}",
            Meta().EntityName(),
            MakeKeyEquation(Meta().PrimaryKey));

        auto statement = database_.PrepareStatement(sql);
        MetaType::PrimaryKeyType::BindValue(statement, 1, primary_key);

        statement.Step();
    }

private:
    void InsertOrReplace(const E& entity, bool insert) {

        auto sql = GetInsertOrReplaceSQL(insert);

        auto statement = database_.PrepareStatement(sql);
        BindEntityValuesToStatement(statement, 1, Meta().Fields(), entity);

        statement.Step();
    }


    static std::string_view GetInsertOrReplaceSQL(bool insert) {
        if (insert) {
            static auto sql = GenerateInsertOrReplaceSQL(true);
            return sql;
        }
        else {
            static auto sql = GenerateInsertOrReplaceSQL(false);
            return sql;
        }
    }


    static std::string GenerateInsertOrReplaceSQL(bool insert) {
        std::string_view verb = insert ? "insert" : "replace";
        return std::format("{} into {} ({}) values ({})",
            verb,
            Meta().EntityName(),
            JoinFieldNames(Meta().Fields()),
            JoinPlaceholders(Meta().Fields().size()));
    }


    static void BindEntityValuesToStatement(
        Statement& statement,
        int begin_index,
        const std::vector<EntityField<E>*>& fields,
        const E& entity) {

        for (auto index : zaf::Range{ 0, fields.size() }) {
            fields[index]->BindValueToStatement(
                statement, 
                static_cast<int>(index) + begin_index,
                entity);
        }
    }

    static void GetEntityValuesFromStatement(
        const Statement& statement, 
        const std::vector<EntityField<E>*>& fields,
        E& entity) {

        for (auto index : zaf::Range{ 0, fields.size() }) {
            fields[index]->GetValueFromStatement(statement, static_cast<int>(index), entity);
        }
    }

private:
    Database& database_;
};

}