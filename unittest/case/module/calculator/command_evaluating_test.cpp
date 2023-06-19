#include <gtest/gtest.h>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "module/calculator/command_parsing.h"
#include "module/calculator/evaluate/evaluator.h"

using namespace ra::mod::calculator;

TEST(CommandEvaluatingTest, BinaryExpression) {

    auto test = [](
        const std::wstring& command, 
        const boost::multiprecision::cpp_dec_float_100& expected_result) {
    
        auto parse_result = ParseCommand(command);
        if (!parse_result) {
            return false;
        }

        auto node = Evaluator::Create(parse_result->GetExpressionRootNode());

        EvaluateResult evaluate_result;
        auto evaluate_status = node->Evaluate(evaluate_result);
        if (evaluate_status != EvaluateStatus::Ok) {
            return false;
        }

        return evaluate_result.decimal_value == expected_result;
    };

    ASSERT_TRUE(test(L"10 * 2 ** 3", 80));
    ASSERT_TRUE(test(L"0xffffffff & 0x00ff00ff", 0x00ff00ff));
    ASSERT_TRUE(test(L"0x00f0ff0f | 0x00ff00ff", 0x00ffffff));
    ASSERT_TRUE(test(L"0x00f0ff0f ^ 0x00ff00ff", 0x000ffff0));
    ASSERT_TRUE(test(L"1 << 8", 0x100));
    ASSERT_TRUE(test(L"0x100 >> 8", 1));
}