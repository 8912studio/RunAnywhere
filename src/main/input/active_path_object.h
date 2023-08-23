#pragma once

#include "main/input/active_path_data.h"
#include "main/input/argument_object.h"

namespace ra::main::input {

class ActivePathObject : public ArgumentObject {
public:
    explicit ActivePathObject(std::shared_ptr<ActivePathData> data);
    explicit ActivePathObject(std::wstring text);

    GUID ClassID() const override;
    std::shared_ptr<ArgumentData> CreateData(std::wstring text) override;
    zaf::Color GetBackgroundColor(const zaf::rich_edit::PaintContext& context) override;
    void OnBeforeOpenWindow(const std::shared_ptr<ArgumentObjectWindow>& window) override;
};

}