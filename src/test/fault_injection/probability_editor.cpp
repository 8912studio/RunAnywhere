#include "test/fault_injection/probability_editor.h"
#include "test/fault_injection/probability.h"

namespace ra::test {

void ProbabilityEditor::Initialize() {

    __super::Initialize();

    value_label_ = zaf::Create<zaf::Label>();
    value_label_->SetFixedWidth(20);
    value_label_->SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);
    value_label_->SetTextAlignment(zaf::dwrite::TextAlignment::Center);
    AddChild(value_label_);

    track_bar_ = zaf::Create<zaf::TrackBar>();
    track_bar_->SetMargin(zaf::Frame{ 4, 2, 0, 2 });
    track_bar_->SetMinValue(Probability::MinValue);
    track_bar_->SetMaxValue(Probability::MaxValue);

    Subscriptions() += track_bar_->ValueChangedEvent().Subscribe(
        std::bind(&ProbabilityEditor::OnTrackBarValueChanged, this));

    AddChild(track_bar_);
}


void ProbabilityEditor::SetAccessMethod(zaf::property_grid::AccessMethod) {

}


void ProbabilityEditor::SetValue(const std::shared_ptr<zaf::Object>& object) {

    auto probability = zaf::As<Probability>(object);
    if (!probability) {
        return;
    }

    int value = static_cast<int>(*probability);
    value_label_->SetText(std::to_wstring(value));
    track_bar_->SetValue(value);
}


void ProbabilityEditor::OnTrackBarValueChanged() {

    int new_value = track_bar_->Value();
    value_label_->SetText(std::to_wstring(new_value));

    NotifyValueChanged(zaf::Create<Probability>(new_value));
}

}