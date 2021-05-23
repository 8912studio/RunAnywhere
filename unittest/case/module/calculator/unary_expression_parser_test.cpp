#include <gtest/gtest.h>
#include "module/calculator/parse/unary_expression_parser.h"

using namespace ra::module::calculator;

TEST(UnaryExpressionParserTest, Normal) {

	auto test = [](std::wstring_view op, OperatorNode::Type expected_type) {
	
		std::wstring command{ op.data(), op.length() };
		command.append(L"1");

		ParseContext context{ command };
		ParseResult result;
		auto status = UnaryExpressionParser::Instance()->Parse(context, result);
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

		if (root_node->children.size() != 1) {
			return false;
		}

		auto operand = std::dynamic_pointer_cast<OperandNode>(root_node->children[0]);
		if (!operand) {
			return false;
		}

		return operand->text == L"1";
	};

	ASSERT_TRUE(test(L"+", OperatorNode::Type::Positive));
	ASSERT_TRUE(test(L"-", OperatorNode::Type::Negative));
	ASSERT_TRUE(test(L"~", OperatorNode::Type::Not));
}


TEST(UnaryExpressionParserTest, InvalidExpression) {

	auto test = [](std::wstring_view command) {
	
		ParseContext context{ command };
		ParseResult result;
		auto status = UnaryExpressionParser::Instance()->Parse(context, result);
		return status == ParseStatus::Mismatched;
	};

	ASSERT_TRUE(test(L""));
	ASSERT_TRUE(test(L"*"));
	ASSERT_TRUE(test(L"4"));
	ASSERT_TRUE(test(L"d"));
}


TEST(UnaryExpressionParserTest, TokenLength) {

	auto test = [](std::wstring_view op) {

		ParseContext context{ op };
		ParseResult result;
		auto status = UnaryExpressionParser::Instance()->Parse(context, result);
		if (status != ParseStatus::Error) {
			return false;
		}

		return context.GetLastParsedLength() == op.length();
	};

	ASSERT_TRUE(test(L"+"));
	ASSERT_TRUE(test(L"-"));
	ASSERT_TRUE(test(L"~"));
}