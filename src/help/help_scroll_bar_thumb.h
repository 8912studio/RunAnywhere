#pragma once

#include <zaf/control/scroll_bar_thumb.h>

namespace ra::help {

class HelpScrollBarThumb : public zaf::ScrollBarThumb {
public:
    ZAF_DECLARE_TYPE;

public:
    void Paint(zaf::Canvas& canvas, const zaf::Rect&) override;
};

}