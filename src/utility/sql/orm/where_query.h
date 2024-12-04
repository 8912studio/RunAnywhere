#pragma once

namespace ra::utility::sql {

template<typename Q, typename Expression>
class WhereQuery {
private:
    using ThisQueryType = WhereQuery<Q, Expression>;

public:
    WhereQuery(Q&& query, Expression&& expression) {

    }

    void Execute() {

    }

    LimitQuery<ThisQueryType> Limit(std::size_t limit) {
        return LimitQuery<ThisQueryType>(*this, limit);
    }
};

}