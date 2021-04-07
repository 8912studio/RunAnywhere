#include <gtest/gtest.h>
#include "module/calculator/parse/expression_parser.h"

using namespace calculator;

TEST(ExpressionParserTest, LeftToRightAssociativity) {

    ParseContext parse_context{ L"1-1-1-1" };
    ParseResult parse_result;
    auto parse_status = ExpressionParser::Instance()->Parse(parse_context, parse_result);
    ASSERT_EQ(parse_status, ParseStatus::Ok);

    //First level
    auto current_node = std::dynamic_pointer_cast<OperatorNode>(parse_result.GetExpressionRootNode());
    ASSERT_NE(current_node, nullptr);
    ASSERT_EQ(current_node->type, OperatorNode::Type::Minus);
    ASSERT_EQ(current_node->children.size(), 2);

    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperatorNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(first_child->type, OperatorNode::Type::Minus);
        ASSERT_EQ(first_child->children.size(), 2);

        current_node = first_child;
    }

    //Second level
    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperatorNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(first_child->type, OperatorNode::Type::Minus);
        ASSERT_EQ(first_child->children.size(), 2);

        current_node = first_child;
    }

    //Third level
    {
        auto second_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[1]);
        ASSERT_NE(second_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");

        auto first_child = std::dynamic_pointer_cast<OperandNode>(current_node->children[0]);
        ASSERT_NE(first_child, nullptr);
        ASSERT_EQ(second_child->text, L"1");
    }
}