#include <gtest/gtest.h>
#include "utility/command_line.h"

using namespace ra::utility;

namespace {

CommandLine::ObjectTextGetter ToObjectTextGetter(const std::vector<std::wstring>& texts) {
    return [texts](int index) {
        return texts[index];
    };
}

}


TEST(CommandLineTest, GetAllParts) {

    {
        CommandLine command_line{ L"" };
        ASSERT_EQ(command_line.AllParts(), std::vector<std::wstring>{});
    }

    {
        CommandLine command_line{ L"  " };
        ASSERT_EQ(command_line.AllParts(), std::vector<std::wstring>{});
    }

    {
        CommandLine command_line{ L"Cmd" };
        ASSERT_EQ(command_line.AllParts(), std::vector<std::wstring>{ L"Cmd" });
    }

    {
        CommandLine command_line{ L"Cmd arg1" };
        std::vector<std::wstring> expected{ L"Cmd", L"arg1" };
        ASSERT_EQ(command_line.AllParts(), expected);
    }

    {
        CommandLine command_line{ L"Cmd arg1 arg2" };
        std::vector<std::wstring> expected{ L"Cmd", L"arg1", L"arg2" };
        ASSERT_EQ(command_line.AllParts(), expected);
    }
}


TEST(CommandLineTest, ObjectOnly) {

    {
        CommandLine command_line{ L"\ufffc", ToObjectTextGetter({ L"o" }) };
        ASSERT_EQ(command_line.Command(), L"o");
        ASSERT_TRUE(command_line.Arguments().empty());
    }

    {
        CommandLine command_line{ L"\ufffc\ufffc", ToObjectTextGetter({ L"11", L"22" })};
        ASSERT_EQ(command_line.Command(), L"11");
        ASSERT_EQ(command_line.Arguments().size(), 1);
        ASSERT_EQ(command_line.Arguments().front(), L"22");
    }

    {
        CommandLine command_line{ L"\ufffc\ufffc \ufffc", ToObjectTextGetter({ 
            L"11", L"22", L"33"
        })};
        ASSERT_EQ(command_line.Command(), L"11");
        ASSERT_EQ(command_line.Arguments().size(), 2);
        ASSERT_EQ(command_line.Arguments()[0], L"22");
        ASSERT_EQ(command_line.Arguments()[1], L"33");
    }
}


TEST(CommandLineTest, TextAndObject) {

    {
        CommandLine command_line{ L"aaa\ufffc", ToObjectTextGetter({ L"b" })};
        ASSERT_EQ(command_line.Command(), L"aaa");
        ASSERT_EQ(command_line.Arguments().size(), 1);
        ASSERT_EQ(command_line.Arguments().front(), L"b");
    }

    {
        CommandLine command_line{ L"\ufffcccc", ToObjectTextGetter({ L"b" }) };
        ASSERT_EQ(command_line.Command(), L"b");
        ASSERT_EQ(command_line.Arguments().size(), 1);
        ASSERT_EQ(command_line.Arguments().front(), L"ccc");
    }

    {
        CommandLine command_line{ L"aaa\ufffcccc", ToObjectTextGetter({ L"b" }) };
        ASSERT_EQ(command_line.Command(), L"aaa");
        ASSERT_EQ(command_line.Arguments().size(), 2);
        ASSERT_EQ(command_line.Arguments()[0], L"b");
        ASSERT_EQ(command_line.Arguments()[1], L"ccc");
    }

    {
        CommandLine command_line{ L"aaa \ufffc ccc \ufffc", ToObjectTextGetter({ L"b", L"dd"})};
        ASSERT_EQ(command_line.Command(), L"aaa");
        ASSERT_EQ(command_line.Arguments().size(), 3);
        ASSERT_EQ(command_line.Arguments()[0], L"b");
        ASSERT_EQ(command_line.Arguments()[1], L"ccc");
        ASSERT_EQ(command_line.Arguments()[2], L"dd");
    }
}


TEST(CommandLineTest, EmptyObjectTest) {

    {
        CommandLine command_line{ L"11\ufffc" };
        ASSERT_EQ(command_line.Command(), L"11");
        ASSERT_EQ(command_line.Arguments().size(), 0);
    }

    {
        CommandLine command_line{ L"\ufffc11" };
        ASSERT_EQ(command_line.Command(), L"11");
        ASSERT_EQ(command_line.Arguments().size(), 0);
    }

    {
        CommandLine command_line{ L"1 \ufffc 22 \ufffc" };
        ASSERT_EQ(command_line.Command(), L"1");
        ASSERT_EQ(command_line.Arguments().size(), 1);
        ASSERT_EQ(command_line.Arguments()[0], L"22");
    }
}