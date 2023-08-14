#include <gtest/gtest.h>
#include "utility/command_line.h"

using namespace ra::utility;

namespace {

CommandLine::ObjectTextGetter ToObjectTextGetter(const std::vector<std::wstring>& texts) {
    return [texts](int index) {
        return CommandLinePiece{ CommandLinePieceType::TextBlock, texts[index] };
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
        ASSERT_TRUE(command_line1.AllPieces().empty());

        ASSERT_EQ(command_line2.RawText(), L"cmd a1 a2 a3");
        std::vector<CommandLinePiece> expected{ { L"cmd" }, { L"a1" }, { L"a2" }, { L"a3" } };
        ASSERT_EQ(command_line2.AllPieces(), expected);
    }

    //Move assign
    {
        CommandLine command_line1{ L"p0 p1 p2" };
        CommandLine command_line2{ L"q0 q1 q2" };
        command_line2 = std::move(command_line1);

        ASSERT_TRUE(command_line1.RawText().empty());
        ASSERT_TRUE(command_line1.AllPieces().empty());

        ASSERT_EQ(command_line2.RawText(), L"p0 p1 p2");
        std::vector<CommandLinePiece> expected{ { L"p0" }, { L"p1" }, { L"p2" } };
        ASSERT_EQ(command_line2.AllPieces(), expected);
    }
}


TEST(CommandLineTest, InvalidObjectTextGetter) {

    auto object_text_getter = [](int) {
        return CommandLinePiece{ CommandLinePieceType::NormalText, L"abc" };
    };
    ASSERT_THROW(CommandLine(L"\ufffc", object_text_getter), std::logic_error);
}


TEST(CommandLineTest, Parse) {

    {
        CommandLine command_line{ L"" };
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        ASSERT_TRUE(command_line.AllPieces().empty());
    }

    {
        CommandLine command_line{ L"  " };
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        ASSERT_TRUE(command_line.AllPieces().empty());
    }

    {
        CommandLine command_line{ L"Cmd" };
        ASSERT_EQ(command_line.Command(), L"Cmd");
        ASSERT_TRUE(command_line.Arguments().empty());
        std::vector<CommandLinePiece> expected{ { L"Cmd" } };
        ASSERT_EQ(command_line.AllPieces(), expected);
    }

    {
        CommandLine command_line{ L"Cmd arg1" };
        ASSERT_EQ(command_line.Command(), L"Cmd");
        std::vector<CommandLinePiece> expected_arguments{ { L"arg1" } };
        ASSERT_EQ(command_line.Arguments(), expected_arguments);
        std::vector<CommandLinePiece> expected_all_pieces{ { L"Cmd" }, { L"arg1" } };
        ASSERT_EQ(command_line.AllPieces(), expected_all_pieces);
    }

    {
        CommandLine command_line(L"Cmd arg1 \ufffc arg3", [](int) {
            return CommandLinePiece{ CommandLinePieceType::TextBlock, L"arg2" };
        });
        ASSERT_EQ(command_line.Command(), L"Cmd");
        std::vector<CommandLinePiece> expected_arguments{
            { CommandLinePieceType::NormalText, L"arg1" },
            { CommandLinePieceType::TextBlock, L"arg2" },
            { CommandLinePieceType::NormalText, L"arg3" },
        };
        ASSERT_EQ(command_line.Arguments(), expected_arguments);
        std::vector<CommandLinePiece> expected_all_pieces{
            { CommandLinePieceType::NormalText, L"Cmd" },
            { CommandLinePieceType::NormalText, L"arg1" },
            { CommandLinePieceType::TextBlock, L"arg2" },
            { CommandLinePieceType::NormalText, L"arg3" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_pieces);
    }

    {
        CommandLine command_line{ L"\ufffc", ToObjectTextGetter({ L"o" }) };
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::TextBlock, L"o" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"\ufffc\ufffc", ToObjectTextGetter({ L"11", L"22" })};
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::TextBlock, L"11" },
            { CommandLinePieceType::TextBlock, L"22" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"\ufffc\ufffc \ufffc", ToObjectTextGetter({ 
            L"11", L"22", L"33"
        })};
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::TextBlock, L"11" },
            { CommandLinePieceType::TextBlock, L"22" },
            { CommandLinePieceType::TextBlock, L"33" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"aaa\ufffc", ToObjectTextGetter({ L"b" })};
        ASSERT_EQ(command_line.Command(), L"aaa");
        std::vector<CommandLinePiece> expected_argument{
            { CommandLinePieceType::TextBlock, L"b" },
        };
        ASSERT_EQ(command_line.Arguments(), expected_argument);
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::NormalText, L"aaa" },
            { CommandLinePieceType::TextBlock, L"b" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"\ufffcccc", ToObjectTextGetter({ L"b" }) };
        ASSERT_TRUE(command_line.Command().empty());
        ASSERT_TRUE(command_line.Arguments().empty());
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::TextBlock, L"b" },
            { CommandLinePieceType::NormalText, L"ccc" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"aaa\ufffcccc", ToObjectTextGetter({ L"b" }) };
        ASSERT_EQ(command_line.Command(), L"aaa");
        std::vector<CommandLinePiece> expected_argument{
            { CommandLinePieceType::TextBlock, L"b" },
            { CommandLinePieceType::NormalText, L"ccc" },
        };
        ASSERT_EQ(command_line.Arguments(), expected_argument);
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::NormalText, L"aaa" },
            { CommandLinePieceType::TextBlock, L"b" },
            { CommandLinePieceType::NormalText, L"ccc" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
    }

    {
        CommandLine command_line{ L"aaa \ufffc ccc \ufffc", ToObjectTextGetter({ L"b", L"dd"})};
        ASSERT_EQ(command_line.Command(), L"aaa");
        std::vector<CommandLinePiece> expected_argument{
            { CommandLinePieceType::TextBlock, L"b" },
            { CommandLinePieceType::NormalText, L"ccc" },
            { CommandLinePieceType::TextBlock, L"dd" },
        };
        ASSERT_EQ(command_line.Arguments(), expected_argument);
        std::vector<CommandLinePiece> expected_all_piece{
            { CommandLinePieceType::NormalText, L"aaa" },
            { CommandLinePieceType::TextBlock, L"b" },
            { CommandLinePieceType::NormalText, L"ccc" },
            { CommandLinePieceType::TextBlock, L"dd" },
        };
        ASSERT_EQ(command_line.AllPieces(), expected_all_piece);
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
        ASSERT_EQ(command_line.Arguments()[0], CommandLinePiece(L"22"));
    }
}