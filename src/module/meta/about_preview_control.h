#pragma once

#include "about/about_view.h"
#include "module/command_preview_control.h"

namespace ra::mod::meta {

class AboutPreviewControl : public CommandPreviewControl {
protected:
    void Initialize() override;

private:
    std::shared_ptr<AboutView> about_view_;
};

}