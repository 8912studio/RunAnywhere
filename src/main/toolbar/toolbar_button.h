#pragma once

#include <zaf/control/button.h>

namespace ra {

class ToolbarButton : public zaf::Button {
public:
    ZAF_DECLARE_TYPE;

    void SetImageName(const std::wstring& name);
    
protected:
    void Initialize() override;
};

}