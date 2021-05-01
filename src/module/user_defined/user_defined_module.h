#pragma once

#include <optional>
#include <vector>
#include "module/module.h"
#include "module/user_defined/user_defined_entry.h"

namespace ra::module::user_defined {

class UserDefinedModule : public Module {
public:
    void Reload();

    std::shared_ptr<Command> Interpret(const std::wstring& command_text) override;

private:
    std::optional<UserDefinedEntry> FindEntry(std::wstring_view keyword);

private:
    std::vector<UserDefinedEntry> entries_;
};

}