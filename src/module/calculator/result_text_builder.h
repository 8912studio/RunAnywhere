#pragma once

#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"

class ResultTextBuilder {
public:
	ResultTextBuilder(
		const calculator::EvaluateResult& evaluate_result, 
		const calculator::Modifier& modifier);

	ResultTextBuilder(const ResultTextBuilder&) = delete;
	ResultTextBuilder& operator=(const ResultTextBuilder&) = delete;

	std::wstring Build() const;

private:
	std::wstring GetDecimalText() const;
	std::wstring GetNonDecimalText() const;

private:
	const calculator::EvaluateResult& evaluate_result_;
	const calculator::Modifier& modifier_;
};