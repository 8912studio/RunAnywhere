#pragma once

#include <format>
#include "utility/sql/orm/base_query.h"
#include "utility/sql/orm/decorative_query_core.h"

namespace ra::utility::sql {

template<typename InnerCore>
class LimitQuery : public BaseQuery<LimitQuery<InnerCore>> {
private:
    friend class BaseQuery<LimitQuery<InnerCore>>;

    class Core : public DecorativeQueryCore<InnerCore> {
    public:
        Core(InnerCore inner_core, std::size_t limit) : 
            DecorativeQueryCore<InnerCore>(std::move(inner_core)),
            limit_(limit) {

        }

        std::string BuildSQL() const {
            return this->GetInnerCore().BuildSQL() + std::format(" limit {}", limit_);
        }

    private:
        std::size_t limit_{};
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    LimitQuery(InnerCore inner_core, std::size_t limit) : core_(std::move(inner_core), limit) {

    }
};

}