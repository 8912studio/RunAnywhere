#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/update/updater.h"

namespace ra::utility::sql {

template<typename InnerCore, typename Expr>
class WhereUpdater : public Updater<WhereUpdater<InnerCore, Expr>> {
public:
    WhereUpdater(InnerCore inner_core, Expr expression) : 
        core_(std::move(inner_core), std::move(expression)) {

    }

private:
    friend class Updater<WhereUpdater<InnerCore, Expr>>;

    class Core {
    public:
        Core(InnerCore&& inner_core, Expr&& expression) :
            inner_core_(std::move(inner_core)),
            expression_(std::move(expression)) {

        }

        Database& GetDB() const {
            return inner_core_.GetDB();
        }

        std::string BuildSQL() const {
            static const auto sql = [this]() {
                auto result = inner_core_.BuildSQL();
                result += std::format(" where {}", expression_.BuildSQL());
                return result;
            }();
            return sql;
        }

        int BindParameters(Statement& statement, int begin_index) const {
            int index = inner_core_.BindParameters(statement, begin_index);
            return expression_.BindParameters(statement, index);
        }

    private:
        InnerCore inner_core_;
        Expr expression_{};
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;
};

}