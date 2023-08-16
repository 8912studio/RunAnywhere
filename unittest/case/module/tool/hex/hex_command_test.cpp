#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/tool/hex/hex_command.h"
#include "module/tool/hex/hex_preview_control.h"

using namespace ra::context;
using namespace ra::mod;
using namespace ra::mod::tool::hex;
using namespace ra::utility;

TEST(HexCommandTest, ShowFileContent) {

    std::filesystem::path current_file_path = __FILEW__;

    auto text = [&current_file_path](const CommandLine& command_line, std::wstring_view expected_file_path) {

        DesktopContext context;
        context.active_path = ActivePath(current_file_path);

        HexCommand hex_command;
        if (!hex_command.Interpret(command_line, context, false)) {
            return false;
        }

        auto preview_control = zaf::As<HexPreviewControl>(hex_command.GetPreviewControl());
        if (!preview_control) {
            return false;
        }

        return preview_control->GetFilePath() == expected_file_path;
    };

    ASSERT_TRUE(text(CommandLine(L"hex"), current_file_path.wstring()));
    ASSERT_TRUE(text(
        CommandLine(L"hex @=C:\\Windows\\explorer.exe"), 
        L"C:\\Windows\\explorer.exe"));
    ASSERT_TRUE(text(
        CommandLine(L"hex @.."),
        current_file_path.parent_path().parent_path().wstring()));
}