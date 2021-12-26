#include "module/calculator/parse/parse_reader.h"
#include <cwctype>
#include "module/calculator/parse/parse_context.h"

namespace ra::module::calculator{

ParseReader::ParseReader(ParseContext& context) : context_(&context) {

}


ParseReader::~ParseReader() {

	Commit();
}


ParseReader::ParseReader(ParseReader&& other) : context_(other.context_), offset_(other.offset_) {

	other.context_ = nullptr;
	other.offset_ = 0;
}


ParseReader& ParseReader::operator=(ParseReader&& other) {

	Commit();

	this->context_ = other.context_;
	this->offset_ = other.offset_;

	other.context_ = nullptr;
	other.offset_ = 0;
	return *this;
}


wchar_t ParseReader::GetChar() const {

	if (!context_) {
		return 0;
	}

	auto ch = context_->GetCharAtOffset(static_cast<int>(offset_));
	if (ch) {
		return *ch;
	}
	return 0;
}


bool ParseReader::Forward() {

	if (context_->GetCurrentIndex() + offset_ == context_->GetLength()) {
		return false;
	}

	++offset_;
	return true;
}


void ParseReader::SkipWhiteSpaces() {

	do {

		if (!std::iswspace(GetChar())) {
			break;
		}
	}
	while (Forward());
}


void ParseReader::Discard() {

	context_ = nullptr;
	offset_ = 0;
}


void ParseReader::Commit() {

	if (context_ && offset_ > 0) {
		context_->Forward(offset_);
	}
}

}