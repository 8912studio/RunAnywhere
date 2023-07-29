#pragma once

namespace ra::mod {

class CommandExecutor {
public:
    CommandExecutor() = default;
    virtual ~CommandExecutor() = default;

    virtual void Execute() = 0;
};

}