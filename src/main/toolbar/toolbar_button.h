#pragma once

#include <zaf/control/button.h>
#include <zaf/object/property_support.h>

namespace ra {

class ToolbarButton : public zaf::Button {
public:
    ZAF_OBJECT;

    void SetImageName(const std::wstring& name);
    
protected:
    void Initialize() override;
};

ZAF_OBJECT_BEGIN(ToolbarButton)
ZAF_OBJECT_PROPERTY(ImageName)
ZAF_OBJECT_END;

}