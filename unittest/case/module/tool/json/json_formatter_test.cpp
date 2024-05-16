#include <gtest/gtest.h>
#include "module/tool/json/json_formatter.h"

using namespace ra::mod::tool::json;

TEST(JSONFormatterTest, PrimitiveFormatter) {

    boost::json::object object;
    object["string"] = "string\" value";
    object["int"] = -100;
    object["uint"] = std::uint32_t(100);

    boost::json::array arr;
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    object["array"] = arr;

    object["double"] = 0.5;

    boost::json::object embedded;
    embedded["double2"] = 1.0;
    embedded["int2"] = 101;
    object["embedded"] = embedded;

    JSONPrimitiveFormatter formatter;
    auto result = formatter.Format(object);

    auto expected =
        R"({"string":"string\" value","int":-100,"uint":100,"array":[1,2,3],"double":0.5,)"
        R"("embedded":{"double2":1.0,"int2":101}})";

    ASSERT_EQ(result, expected);
}