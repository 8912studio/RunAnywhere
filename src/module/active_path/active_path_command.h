#pragma once

#include "module/command.h"
#include "module/active_path/active_path_option.h"

namespace ra::module::active_path {

class ActivePathCommand : public Command {
public:
    explicit ActivePathCommand(const ActivePathOption& option);

    help::content::Content GetHelpContent() override;
    std::wstring GetPreviewText() override;
    void Execute() override;

private:
    ActivePathOption option_;
};

}