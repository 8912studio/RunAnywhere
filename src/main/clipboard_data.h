#pragma once

#include <zaf/clipboard/clipboard_data.h>
#include <zaf/object/object.h>

namespace ra {

class ClipboardData : public zaf::clipboard::ClipboardData {
public:
    explicit ClipboardData(const std::vector<std::shared_ptr<zaf::Object>>& objects);

    zaf::clipboard::Medium SaveToMedium(const zaf::clipboard::Format& format) override;
    void LoadFromMedium(
        const zaf::clipboard::Format& format, 
        const zaf::clipboard::Medium& medium) override;

private:
    std::vector<std::shared_ptr<zaf::Object>> objects_;
};

}