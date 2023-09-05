#pragma once

#include <cstddef>
#include <optional>

namespace ra::mod::calculator{

class ParseContext;

class ParseReader {
public:
	ParseReader(ParseContext& context);
	~ParseReader();

	ParseReader(const ParseReader&) = delete;
	ParseReader& operator=(const ParseReader&) = delete;

	ParseReader(ParseReader&&);
	ParseReader& operator=(ParseReader&&);

	wchar_t GetChar() const;

	bool IsAtLineEnd() const;

	bool Forward();
	
	void SkipWhiteSpaces();
	void SkipSpaces();

	void Discard();

private:
	void Commit();

private:
	ParseContext* context_{};
	std::size_t offset_{};
};

}