#pragma once

#include <zaf/base/range.h>
#include "utility/sql/database.h"
#include "utility/sql/entity.h"

namespace ra::utility::sql {

template<typename T>
class Table {
public:
    explicit Table(Database& database) : database_(database) {

    }

    std::vector<T> SelectAll() {


    }

    std::optional<T> Select(const Entity<T>::PrimaryKeyType::ValueType& primary_key) {

        const auto& properties = Entity<T>::Instance().Properties();

        std::string sql("select ");
        for (auto index : zaf::Range(0, properties.size())) {
            if (index != 0) {
                sql.append(",");
            }
            sql.append(properties[index]->Name());
        }

        sql.append(" from ").append(Entity<T>::Instance().EntityName());

        sql.append(" where (");

        const auto& primary_key_properties = Entity<T>::Instance().PrimaryKey.Properties();
        for (auto index : zaf::Range(0, primary_key_properties.size())) {
            if (index != 0) {
                sql.append(",");
            }
            sql.append(primary_key_properties[index]->Name());
        }
        sql.append(") = (");

        for (auto index : zaf::Range(0, primary_key_properties.size())) {
            if (index != 0) {
                sql.append(",");
            }
            sql.append("?");
        }
        sql.append(")");

        auto statement = database_.PrepareStatement(sql);

        for (auto index : zaf::Range(0, primary_key_properties.size())) {
            statement.BindParameter(static_cast<int>(index) + 1, primary_key);
        }

        if (!statement.Step()) {
            return std::nullopt;
        }

        T result{};
        for (auto index : zaf::Range(0, properties.size())) {
            properties[index]->GetValueFromStatement(statement, static_cast<int>(index), result);
        }
        return result;
    }


    void Insert(const T& entity) {

        std::string sql("insert into ");
        sql.append(Entity<T>::Instance().EntityName());
        sql.append("(");

        const auto& properties = Entity<T>::Instance().Properties();
        for (auto index : zaf::Range(0, properties.size())) {
            if (index != 0) {
                sql.append(",");
            }
            sql.append(properties[index]->Name());
        }

        sql.append(") values (");

        for (auto index : zaf::Range(0, properties.size())) {
            if (index != 0) {
                sql.append(",");
            }
            sql.append("?");
        }
        sql.append(")");

        auto statement = database_.PrepareStatement(sql);

        for (auto index : zaf::Range(0, properties.size())) {
            properties[index]->BindValueToStatement(statement, static_cast<int>(index) + 1, entity);
        }

        statement.Step();
    }

    void Update(const T& entity) {

    }

private:
    Database& database_;
};

}