#pragma once

#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"

class ResultText {
public:
	std::wstring GetCompleteText() const {
		return prefix + content;
	}

public:
	std::wstring prefix;
	std::wstring content;
};

class ResultTextBuilder {
public:
	ResultTextBuilder(
		const calculator::EvaluateResult& evaluate_result, 
		const calculator::Modifier& modifier);

	ResultTextBuilder(const ResultTextBuilder&) = delete;
	ResultTextBuilder& operator=(const ResultTextBuilder&) = delete;

	ResultText Build() const;

private:
	std::wstring GetDecimalText() const;
	std::wstring GetNonDecimalText() const;

private:
	const calculator::EvaluateResult& evaluate_result_;
	const calculator::Modifier& modifier_;
};