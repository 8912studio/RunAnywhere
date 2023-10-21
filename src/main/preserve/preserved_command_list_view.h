#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include "main/preserve/preserved_command_view.h"
#include "utility/thin_scroll_control.h"

namespace ra {

class PreservedCommandListView : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void AddView(const std::shared_ptr<PreservedCommandView>& view);
    void RemoveView(const std::shared_ptr<PreservedCommandView>& view);
    void RemoveFirstView();

    std::size_t ViewCount() const;

protected:
    void AfterParse() override;

private:
    ZAF_BIND_CONTROL(utility::ThinScrollControl, scrollControl);
};

}