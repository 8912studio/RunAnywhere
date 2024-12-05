#pragma once

#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename InnerCore>
class DecorativeQueryCore {
public:
    using EntityType = typename InnerCore::EntityType;
    using ResultElementType = typename InnerCore::ResultElementType;

public:
    DecorativeQueryCore(InnerCore inner_core) : inner_core_(std::move(inner_core)) {

    }

    Statement PrepareStatement(std::string_view sql) const {
        return inner_core_.PrepareStatement(sql);
    }

    int BindParameters(Statement& statement, int begin_index) const {
        return inner_core_.BindParameters(statement, begin_index);
    }

    ResultElementType GetElement(const Statement& statement) const {
        return inner_core_.GetElement(statement);
    }

protected:
    const InnerCore& GetInnerCore() const {
        return inner_core_;
    }

private:
    InnerCore inner_core_;
};

}