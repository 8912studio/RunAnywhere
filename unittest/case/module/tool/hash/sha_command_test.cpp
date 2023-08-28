#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/tool/hash/sha_command.h"

using namespace ra::context;
using namespace ra::mod::tool::hash;
using namespace ra::utility;

TEST(SHACommandTest, SHACommand) {

    auto test = [](const std::wstring& command_text, const std::wstring& expected_result) {

        CommandLine command_line{ command_text };

        DesktopContext context;
        context.active_path = ActivePath{ L"C:\\windows\\notepad.exe" };

        SHACommand command;
        if (!command.Interpret(command_line, context, false)) {
            return false;
        }
        auto preview_control = zaf::As<HashPreviewControl>(command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }
        return preview_control->GetText() == expected_result;
    };

    ASSERT_TRUE(test(L"sha zaf /1", L"eaee311eb87bd13bd59f3041486a678cf556905c"));
    ASSERT_TRUE(test(
        L"sha zaf /256", 
        L"ad38816045829dfd1c07de7d18954a0d63ff3f7bd7602adba33a0e95e7f5d4f6"));
    ASSERT_TRUE(test(
        L"sha zaf /384", 
        L"b26397fabbb5bd1b2ee81248c03ea6ec07db6ffa9c570ec5"
        L"3f9ce8e9f46f57625793cbfca5c6c72cf2fe0cbba188d5cf"));
    ASSERT_TRUE(test(
        L"sha zaf /512",
        L"029546fa5d6dc7fa04e24c5d4073dcd6670f83fb2f26d545716f68e928c88588"
        L"989cf466d1bf2a5fbde1df9a5fe6d2e1715ceebbd4a62c45accb2403599450d5"));
}