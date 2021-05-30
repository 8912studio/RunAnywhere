#include "utility/command_line_arguments.h"
#include <Windows.h>
#include <zaf/base/error/check.h>

namespace ra::utility {

CommandLineArguments CommandLineArguments::Parse(const std::wstring& command_line) {

	int count{};
	auto arguments = CommandLineToArgvW(command_line.c_str(), &count);
	return CommandLineArguments{ arguments, count };
}


CommandLineArguments::CommandLineArguments(wchar_t** arguments, int count) : 
	arguments_(arguments),
	count_(count) {

}


CommandLineArguments::~CommandLineArguments() {

	FreeArguments();
}


CommandLineArguments::CommandLineArguments(CommandLineArguments&& other) : 
	arguments_(other.arguments_),
	count_(other.count_) {

	other.FreeArguments();
}


CommandLineArguments& CommandLineArguments::operator=(CommandLineArguments&& other) {

	FreeArguments();

	arguments_ = other.arguments_;
	count_ = other.count_;

	other.FreeArguments();
	return *this;
}


void CommandLineArguments::FreeArguments() {

	if (arguments_) {

		LocalFree(arguments_);

		arguments_ = nullptr;
		count_ = 0;
	}
}


std::wstring CommandLineArguments::operator[](int index) const {

	ZAF_EXPECT(index >= 0 && index < count_);
	return arguments_[index];
}

}