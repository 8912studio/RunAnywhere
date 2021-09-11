#pragma once

#include <zaf/control/control.h>
#include <zaf/rx/subject.h>
#include "help/content/content.h"
#include "help/content_layouter.h"

namespace ra::help {

class HelpContentControl : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

    void SetContent(const content::Content& content);

    zaf::Observable<float> NeedUpdateHeightEvent() const {
        return need_update_height_event_.GetObservable();
    }

protected:
    void AfterParse() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect&) override;
    void OnRectChanged(const zaf::Rect& previous_rect) override;
    void UpdateHeight();

private:
    ContentLayouter content_layouter_;

    zaf::Subject<float> need_update_height_event_;
};

}