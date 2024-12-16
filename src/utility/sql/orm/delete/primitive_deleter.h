#pragma once

#include <format>
#include "utility/sql/database.h"
#include "utility/sql/orm/delete/deleter.h"
#include "utility/sql/orm/delete/where_deleter.h"

namespace ra::utility::sql {

template<typename E>
class PrimitiveDeleter : public Deleter<PrimitiveDeleter<E>> {
private:
    friend class Deleter<PrimitiveDeleter<E>>;

    class Core {
    public:
        explicit Core(Database& db) : db_(db) {

        }

        std::string BuildSQL() const {

            static const std::string sql = [this]() {
                return std::format("delete from {}", E::TableType::GetInstance().GetName());
            }();

            return sql;
        }

        Statement PrepareStatement(std::string_view sql) const {
            return db_.PrepareStatement(sql);
        }

        int BindParameters(Statement& statement, int begin_index) const {
            return begin_index;
        }

    private:
        Database& db_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    explicit PrimitiveDeleter(Database& db) : core_(db) {

    }

    template<typename Expr>
    auto Where(Expr expression) const {
        return WhereDeleter<Core, Expr>(core_, std::move(expression));
    }
};

}