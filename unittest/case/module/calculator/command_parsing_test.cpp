#include <gtest/gtest.h>
#include "module/calculator/command_parsing.h"

using namespace ra::module::calculator;

namespace {

bool AreSyntaxNodesEqual(const SyntaxNode& node1, const SyntaxNode& node2);


bool AreOperandsEqual(const OperandNode& node1, const OperandNode& node2) {

    return
        node1.text == node2.text &&
        node1.unit == node2.unit &&
        node1.base == node2.base;
}


bool AreOperatorsEqual(const OperatorNode& node1, const OperatorNode& node2) {

    if (node1.type != node2.type) {
        return false;
    }

    if (node1.children.size() != node2.children.size()) {
        return false;
    }

    for (std::size_t index = 0; index < node1.children.size(); ++index) {
        if (!AreSyntaxNodesEqual(*node1.children[index], *node2.children[index])) {
            return false;
        }
    }

    return true;
}


bool AreSyntaxNodesEqual(const SyntaxNode& node1, const SyntaxNode& node2) {

    auto operand1 = dynamic_cast<const OperandNode*>(&node1);
    if (operand1) {

        auto operand2 = dynamic_cast<const OperandNode*>(&node2);
        if (operand2) {
            return AreOperandsEqual(*operand1, *operand2);
        }
        else {
            return false;
        }
    }

    auto operator1 = dynamic_cast<const OperatorNode*>(&node1);
    if (operator1) {

        auto operator2 = dynamic_cast<const OperatorNode*>(&node2);
        if (operator2) {
            return AreOperatorsEqual(*operator1, *operator2);
        }
        else {
            return false;
        }
    }

    return false;
}


bool AreParseResultsEqual(ParseResult& result1, ParseResult& result2) {

    auto node1 = result1.GetExpressionRootNode();
    auto node2 = result2.GetExpressionRootNode();
    if (!AreSyntaxNodesEqual(*node1, *node2)) {
        return false;
    }

    const auto& modifier1 = result1.GetModifier();
    const auto& modifier2 = result2.GetModifier();
    return
        modifier1.base == modifier2.base &&
        modifier1.bit_length == modifier2.bit_length &&
        modifier1.highlight_bit == modifier2.highlight_bit &&
        modifier1.unit == modifier2.unit &&
        modifier1.use_upper_case == modifier2.use_upper_case;
}

}


TEST(CommandParsingTest, ReviseCommand) {

    auto test = [](std::wstring_view command, std::wstring_view expected_command) {

        auto result = ParseCommand(command);
        if (!result) {
            return false;
        }

        auto expected_result = ParseCommand(expected_command);
        if (!expected_result) {
            return false;
        }

        return AreParseResultsEqual(*result, *expected_result);
    };

    ASSERT_TRUE(test(L"1+", L"1"));
    ASSERT_TRUE(test(L"1+1024**", L"1+1024"));
    ASSERT_TRUE(test(L"1+;x", L"1;x"));
    ASSERT_TRUE(test(L"8-0b", L"8"));
    ASSERT_TRUE(test(L"1+(2", L"1+(2)"));
    ASSERT_TRUE(test(L"1+(3(", L"1+(3)"));
    ASSERT_TRUE(test(L"1+(4((", L"1+(4)"));
    ASSERT_TRUE(test(L"1+(((5((", L"1+(((5)))"));
    ASSERT_TRUE(test(L"1+(2+(3+(4+5(", L"1+(2+(3+(4+5)))"));
    ASSERT_TRUE(test(L"(1);", L"(1)"));
    ASSERT_TRUE(test(L"(1;", L"(1)"));
    ASSERT_TRUE(test(L"100*10;b^", L"100*10;b"));
    ASSERT_TRUE(test(L"100*11;'", L"100*11;"));
}