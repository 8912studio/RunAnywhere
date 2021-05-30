#pragma once

#include "module/module.h"

namespace ra::module::date {

class DateModule : public Module {
public:
	std::shared_ptr<Command> Interpret(const std::wstring& command_text) override;
};

}