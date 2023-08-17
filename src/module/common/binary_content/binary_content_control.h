#pragma once

#include <cstddef>
#include <vector>
#include <zaf/control/control_binder.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/common/binary_content/binary_column_body.h"

namespace ra::mod {

class BinaryContentControl : public zaf::VerticalBox {
public:
    ZAF_DECLARE_TYPE;

public:
    const std::vector<std::byte>& GetBinary() const;
    void SetBinary(std::vector<std::byte> binary);
    void ChangeStyle(CommandDisplayStyle style);

protected:
    void AfterParse() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;

private:
    void AdjustBodyHeight();

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(BinaryColumnBody, body);

    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
};

}