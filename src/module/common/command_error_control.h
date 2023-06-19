#pragma once

#include "module/command_preview_control.h"
#include "module/common/error_view.h"

namespace ra::mod {

class CommandErrorControl : public CommandPreviewControl {
public:
    //Gets an error control used for invalid argument situation.
    static std::shared_ptr<CommandErrorControl> InvalidArgument();

public:
    void SetErrorText(const std::wstring& text);

protected:
    void AfterParse() override;

private:
    std::shared_ptr<ErrorView> error_view_;
};

}