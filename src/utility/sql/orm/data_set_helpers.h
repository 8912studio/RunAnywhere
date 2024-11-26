#pragma once

#include <format>
#include <string>
#include <zaf/base/range.h>
#include "utility/sql/orm/column.h"
#include "utility/sql/orm/primary_key.h"

namespace ra::utility::sql {

std::string JoinPlaceholders(std::size_t count);

std::string JoinColumnNames(AbstractColumnsView fields);

std::string MakeKeyEquation(const AbstractPrimaryKey& key);

template<typename E>
void BindEntityValuesToStatement(
    Statement& statement,
    int begin_index,
    std::span<const Column<E>* const> fields,
    const E& entity) {

    for (auto index : zaf::Range{ 0, fields.size() }) {
        fields[index]->BindValueToStatement(
            statement,
            static_cast<int>(index) + begin_index,
            entity);
    }
}

}