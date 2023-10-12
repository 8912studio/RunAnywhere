#pragma once

#include <memory>
#include <string>
#include "module/command_executor.h"

namespace ra::mod {

class CopyExecutor : public CommandExecutor {
public:
    static std::shared_ptr<CopyExecutor> TryCreate(std::wstring text);

public:
    ExecuteResult Execute() override;

private:
    explicit CopyExecutor(std::wstring text);

private:
    std::wstring text_;
};

}