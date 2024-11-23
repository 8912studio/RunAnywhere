#include "utility/sql/orm/data_set_helpers.h"
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>

namespace ra::utility::sql {

std::string JoinPlaceholders(std::size_t count) {

    std::string result;
    for (auto index : zaf::Range{ 0, count }) {
        if (index != 0) {
            result += ',';
        }
        result += '?';
    }
    return result;
}


std::string JoinFieldNames(AbstractFieldsView fields) {

    return zaf::JoinAsString(fields, ",", [](auto field) {
        return field->Name();
    });
}


std::string MakeKeyEquation(const AbstractPrimaryKey& key) {

    auto fields = key.GetAbstractFields();
    return std::format("({})=({})", JoinFieldNames(fields), JoinPlaceholders(fields.size()));
}

}