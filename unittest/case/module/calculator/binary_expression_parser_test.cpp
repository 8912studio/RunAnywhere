#include <gtest/gtest.h>
#include "module/calculator/parse/binary_expression_parser.h"

using namespace ra::module::calculator;

TEST(BinaryExpressionParserTest, Normal) {

	auto test = [](std::wstring_view op, OperatorNode::Type expected_type) {
	
		std::wstring command{ op.data(), op.length() };
		command.append(L"2");

		ParseContext context{ command };

		ParseResult result;
		result.AddOperand(std::make_shared<OperandNode>()); //Fake operand 

		auto status = BinaryExpressionParser::Instance()->Parse(context, result);
		if (status != ParseStatus::Ok) {
			return false;
		}

		auto root_node = std::dynamic_pointer_cast<OperatorNode>(result.GetExpressionRootNode());
		if (!root_node) {
			return false;
		}

		if (root_node->type != expected_type) {
			return false;
		}

		if (root_node->children.size() != 2) {
			return false;
		}

		auto operand = std::dynamic_pointer_cast<OperandNode>(root_node->children[1]);
		if (!operand) {
			return false;
		}

		if (operand->text != L"2") {
			return false;
		}
		
		return true;
	};

	ASSERT_TRUE(test(L"+", OperatorNode::Type::Plus));
	ASSERT_TRUE(test(L"-", OperatorNode::Type::Minus));
	ASSERT_TRUE(test(L"*", OperatorNode::Type::Multiply));
	ASSERT_TRUE(test(L"/", OperatorNode::Type::Divide));
	ASSERT_TRUE(test(L"**", OperatorNode::Type::Power));
	ASSERT_TRUE(test(L"&", OperatorNode::Type::And));
	ASSERT_TRUE(test(L"|", OperatorNode::Type::Or));
	ASSERT_TRUE(test(L"^", OperatorNode::Type::Xor));
	ASSERT_TRUE(test(L"<<", OperatorNode::Type::LeftShift));
	ASSERT_TRUE(test(L">>", OperatorNode::Type::RightShift));
}


TEST(BinaryExpressionParserTest, TokenLength) {

	auto test = [](std::wstring_view op) {

		std::wstring command{ op.data(), op.length() };

		ParseContext context{ command };
		ParseResult result;
		auto status = BinaryExpressionParser::Instance()->Parse(context, result);
		if (status != ParseStatus::Error) {
			return false;
		}

		return context.GetLastParsedLength() == op.length();
	};

	ASSERT_TRUE(test(L"+"));
	ASSERT_TRUE(test(L"-"));
	ASSERT_TRUE(test(L"*"));
	ASSERT_TRUE(test(L"/"));
	ASSERT_TRUE(test(L"**"));
	ASSERT_TRUE(test(L"&"));
	ASSERT_TRUE(test(L"|"));
	ASSERT_TRUE(test(L"^"));
	ASSERT_TRUE(test(L"<<"));
	ASSERT_TRUE(test(L">>"));
}


TEST(BinaryExpressionParserTest, EmptyExpression) {

	auto test = [](std::wstring_view string) {

		ParseContext context{ string };
		ParseResult result;
		auto status = BinaryExpressionParser::Instance()->Parse(context, result);
		if (status != ParseStatus::Ok) {
			return false;
		}

		return context.GetCurrentIndex() == 0;
	};

	ASSERT_TRUE(test(L""));
	ASSERT_TRUE(test(L"443"));
	ASSERT_TRUE(test(L"ikh"));
}


TEST(BinaryExpressionParserTest, ImplicitMultiplyAtTail) {

	ParseContext context{ L"(3)" };

	ParseResult result;
	result.AddOperand(std::make_shared<OperandNode>()); //Fake operand

	auto status = BinaryExpressionParser::Instance()->Parse(context, result);
	ASSERT_EQ(status, ParseStatus::Ok);

	auto root_node = std::dynamic_pointer_cast<OperatorNode>(result.GetExpressionRootNode());
	ASSERT_NE(root_node, nullptr);
	ASSERT_EQ(root_node->type, OperatorNode::Type::Multiply);
	ASSERT_EQ(root_node->children.size(), 2);

	auto operand = std::dynamic_pointer_cast<OperandNode>(root_node->children[1]);
	ASSERT_NE(operand, nullptr);
	ASSERT_EQ(operand->text, L"3");
}


TEST(BinaryExpressionParserTest, ImplicitMultiplyAtHead) {

	ParseContext context{ L")4" };
	context.BeginRead().Forward();

	ParseResult result;
	result.AddOperand(std::make_shared<OperandNode>()); //Fake operand

	auto status = BinaryExpressionParser::Instance()->Parse(context, result);
	ASSERT_EQ(status, ParseStatus::Ok);

	auto root_node = std::dynamic_pointer_cast<OperatorNode>(result.GetExpressionRootNode());
	ASSERT_NE(root_node, nullptr);
	ASSERT_EQ(root_node->type, OperatorNode::Type::Multiply);
	ASSERT_EQ(root_node->children.size(), 2);

	auto operand = std::dynamic_pointer_cast<OperandNode>(root_node->children[1]);
	ASSERT_NE(operand, nullptr);
	ASSERT_EQ(operand->text, L"4");
}