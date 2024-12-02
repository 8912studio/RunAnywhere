#pragma once

#include <span>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/abstract_key.h"

namespace ra::utility::sql {

class AbstractIndex : public AbstractKey {
public:

};

using AbstractIndexesView = std::span<const AbstractIndex* const>;

}