#pragma once

#include <format>
#include <string>
#include <zaf/base/string/join.h>
#include "utility/sql/orm/entity_field.h"
#include "utility/sql/orm/entity_key.h"

namespace ra::utility::sql {

std::string JoinPlaceholders(std::size_t count);

template<typename E>
std::string JoinFieldNames(const std::vector<EntityField<E>*>& fields) {
    return zaf::JoinAsString(fields, ",", [](auto field) {
        return field->Name();
    });
}


template<typename E, typename... Fields>
std::string MakeKeyEquation(const EntityKey<E, Fields...>& key) {
    return std::format(
        "({})=({})", 
        JoinFieldNames(key.Fields()), 
        JoinPlaceholders(key.Fields().size()));
}

}