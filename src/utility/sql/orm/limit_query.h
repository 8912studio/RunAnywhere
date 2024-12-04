#pragma once

namespace ra::utility::sql {

template<typename Q>
class LimitQuery {
public:
    LimitQuery(Q&& query, std::size_t limit) : query_(std::forward<Q>(query)), limit_(limit) {

    }

    void Execute() {

    }

private:
    Q query_;
    std::size_t limit_{};
};

}