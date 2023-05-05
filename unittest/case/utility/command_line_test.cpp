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


static_assert(!std::is_copy_constructible_v<CommandLine>);
static_assert(!std::is_copy_assignable_v<CommandLine>);
static_assert(std::is_move_constructible_v<CommandLine>);
static_assert(std::is_move_assignable_v<CommandLine>);


TEST(CommandLineTest, Move) {

    //Move construct
    {
        CommandLine command_line1{ L"cmd a1 a2 a3" };
        CommandLine command_line2{ std::move(command_line1) };

        ASSERT_TRUE(command_line1.RawText().empty());
        ASSERT_TRUE(command_line1.AllParts().empty());

        ASSERT_EQ(command_line2.RawText(), L"cmd a1 a2 a3");
        std::vector<std::wstring> expected{ L"cmd", L"a1", L"a2", L"a3" };
        ASSERT_EQ(command_line2.AllParts(), expected);
    }

    //Move assign
    {
        CommandLine command_line1{ L"p0 p1 p2" };
        CommandLine command_line2{ L"q0 q1 q2" };
        command_line2 = std::move(command_line1);

        ASSERT_TRUE(command_line1.RawText().empty());
        ASSERT_TRUE(command_line1.AllParts().empty());

        ASSERT_EQ(command_line2.RawText(), L"p0 p1 p2");
        std::vector<std::wstring> expected{ L"p0", L"p1", L"p2" };
        ASSERT_EQ(command_line2.AllParts(), expected);
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


TEST(CommandLineTest, EmptyObjectText) {

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