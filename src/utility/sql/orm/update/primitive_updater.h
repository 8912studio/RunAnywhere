#pragma once

#include "utility/sql/orm/update/primitive_updater_core.h"
#include "utility/sql/orm/update/updater.h"

namespace ra::utility::sql {

template<typename Core>
class PrimitiveUpdater : public Updater<PrimitiveUpdater<Core>> {
public:
    explicit PrimitiveUpdater(Core core) : core_(std::move(core)) {

    }

private:
    friend class Updater<PrimitiveUpdater<Core>>;

    const Core& GetCore() const {
        return core_;
    }

    Core core_;
};

}