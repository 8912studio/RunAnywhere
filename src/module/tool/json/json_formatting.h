#pragma once

#include <boost/json.hpp>

namespace ra::mod::tool::json {

std::string FormatJSONValue(const boost::json::value& value);

}