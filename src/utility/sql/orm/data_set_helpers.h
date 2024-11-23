#pragma once

#include <format>
#include <string>
#include "utility/sql/orm/field.h"
#include "utility/sql/orm/primary_key.h"

namespace ra::utility::sql {

std::string JoinPlaceholders(std::size_t count);

std::string JoinFieldNames(AbstractFieldsView fields);

std::string MakeKeyEquation(const AbstractPrimaryKey& key);

}