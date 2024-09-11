#pragma once

#include "main/input/active_path_data.h"
#include "main/input/argument_object.h"

namespace ra::main::input {

class ActivePathObject : public ArgumentObject {
public:
    explicit ActivePathObject(std::shared_ptr<ActivePathData> data);
    explicit ActivePathObject(std::wstring text);

    std::shared_ptr<ArgumentData> CreateData(std::wstring text) override;
    zaf::Color GetBackgroundColor() const override;
    std::shared_ptr<ArgumentObjectWindow> CreateArgumentObjectWindow() override;

    std::shared_ptr<zaf::textual::InlineObject> Clone() const override;
};

}