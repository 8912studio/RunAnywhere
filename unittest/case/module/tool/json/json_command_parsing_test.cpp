#include <boost/json.hpp>
#include <gtest/gtest.h>
#include "module/tool/json/json_command_parsing.h"

using namespace ra::mod::tool::json;
using namespace ra::utility;

TEST(JSONCommandParsingTest, ExtraData) {

    CommandLine command_line{ L"json {}a" };

    auto result = ParseJSONCommand(command_line);
    auto error = result.Error();
    ASSERT_NE(error, nullptr);
    ASSERT_EQ(error->code, make_error_code(boost::json::error::extra_data));
    ASSERT_EQ(error->last_parsed_index, 2);
    ASSERT_EQ(error->original_text, L"{}a");
}