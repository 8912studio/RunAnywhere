#include "module/meta/about_preview_control.h"
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>

namespace ra::mod::meta {

void AboutPreviewControl::Initialize() {

    __super::Initialize();

    this->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    this->SetAutoHeight(true);

    about_view_ = zaf::Create<AboutView>();
    this->AddChild(about_view_);
}

}