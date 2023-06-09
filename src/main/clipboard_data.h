#pragma once

#include <zaf/clipboard/clipboard_data.h>
#include <zaf/object/object.h>

namespace ra {

class ClipboardData : public zaf::clipboard::ClipboardData {
public:
    static constexpr zaf::clipboard::FormatType FormatType = 
        zaf::clipboard::MakePrivateFormatType(1);

public:
    void AddObject(std::shared_ptr<zaf::Object> object);

    zaf::clipboard::Medium SaveToMedium(const zaf::clipboard::Format& format) override;
    void LoadFromMedium(
        const zaf::clipboard::Format& format, 
        const zaf::clipboard::Medium& medium) override;

private:
    zaf::clipboard::Medium SaveTextToMedium();

private:
    std::vector<std::shared_ptr<zaf::Object>> objects_;
};

}