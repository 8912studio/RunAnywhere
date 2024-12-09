#pragma once

#include <format>
#include "utility/sql/orm/select/selecter.h"
#include "utility/sql/orm/select/decorative_selecter_core.h"

namespace ra::utility::sql {

template<typename InnerCore>
class LimitSelecter : public Selecter<LimitSelecter<InnerCore>> {
public:
    LimitSelecter(InnerCore inner_core, std::size_t limit) : core_(std::move(inner_core), limit) {

    }

private:
    friend class Selecter<LimitSelecter<InnerCore>>;

    class Core : public DecorativeSelecterCore<InnerCore> {
    public:
        Core(InnerCore inner_core, std::size_t limit) : 
            DecorativeSelecterCore<InnerCore>(std::move(inner_core)),
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
};

}