#pragma once

#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"
#include "module/common/error_view.h"

namespace ra::module {

class BasePreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

protected:
    void ShowInvalidArgumentErrorView();

private:
    ZAF_BIND_CONTROL(ErrorView, errorView);
};

}