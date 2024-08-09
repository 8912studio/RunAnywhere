#pragma once

#include <cstddef>
#include <vector>
#include <zaf/base/byte_array.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/scroll_box.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_column_body.h"

namespace ra::mod {

class BinaryContentControl : public zaf::VerticalBox {
public:
    ZAF_OBJECT;

public:
    const zaf::ByteArray& GetBinary() const;
    void SetBinary(zaf::ByteArray binary);
    void ChangeStyle(CommandDisplayStyle style);

protected:
    void AfterParse() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) const override;

private:
    void AdjustBodyHeight();

private:
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollControl);
    ZAF_BIND_CONTROL(BinaryColumnBody, body);

    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
};

ZAF_OBJECT_BEGIN(BinaryContentControl)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/common/binary_content/binary_content_control.xaml")
ZAF_OBJECT_END;

}