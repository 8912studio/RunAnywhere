#pragma once

#include "utility/sql/orm/delete/deleter.h"

namespace ra::utility::sql {

template<typename InnerCore, typename Expr>
class WhereDeleter : public Deleter<WhereDeleter<InnerCore, Expr>> {
private:
    friend class Deleter<WhereDeleter<InnerCore, Expr>>;

    class Core {
    public:
        Core(InnerCore&& core, Expr&& expr) : 
            inner_core_(std::move(core)), 
            expression_(std::move(expr)) {

        }

        std::string BuildSQL() const {
            static const auto sql = [this]() {
                auto result = inner_core_.BuildSQL();
                result += std::format(" where {}", expression_.BuildSQL());
                return result;
            }();
            return sql;
        }

        Statement PrepareStatement(std::string_view sql) const {
            return inner_core_.PrepareStatement(sql);
        }

        int BindParameters(Statement& statement, int begin_index) const {
            int index = inner_core_.BindParameters(statement, begin_index);
            return expression_.BindParameters(statement, index);
        }

    private:
        InnerCore inner_core_;
        Expr expression_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    WhereDeleter(InnerCore core, Expr expr) : core_(std::move(core), std::move(expr)) {

    }
};

}