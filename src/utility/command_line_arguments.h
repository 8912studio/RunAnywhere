#pragma once

#include <string>

namespace ra::utility {

class CommandLineArguments {
public:
	static CommandLineArguments Parse(const std::wstring& command_line);

public:
	~CommandLineArguments();

	CommandLineArguments(const CommandLineArguments&) = delete;
	CommandLineArguments& operator=(const CommandLineArguments&) = delete;

	CommandLineArguments(CommandLineArguments&&);
	CommandLineArguments& operator=(CommandLineArguments&&);

	int GetCount() const {
		return count_;
	}

	std::wstring operator[](int index) const;

private:
	CommandLineArguments(wchar_t** arguments, int count);

	void FreeArguments();

private:
	wchar_t** arguments_;
	int count_{};
};

}