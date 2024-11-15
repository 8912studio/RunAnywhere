#pragma once

#include <zaf/control/label.h>
#include <zaf/control/property_grid/value_editor.h>
#include <zaf/control/track_bar.h>

namespace ra::test {

class ProbabilityEditor : public zaf::property_grid::ValueEditor {
public:
    void SetAccessMethod(zaf::property_grid::AccessMethod) override;
    void SetValue(const std::shared_ptr<zaf::Object>& object) override;

protected:
    void Initialize() override;

private:
    void OnTrackBarValueChanged();

private:
    std::shared_ptr<zaf::Label> value_label_;
    std::shared_ptr<zaf::TrackBar> track_bar_;
};

}