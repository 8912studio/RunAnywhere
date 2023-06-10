#pragma once

#include <zaf/clipboard/clipboard_data.h>
#include <zaf/object/object.h>

namespace ra {

class ClipboardData : public zaf::clipboard::ClipboardData {
public:
    static constexpr zaf::clipboard::FormatType PrivateFormatType = 
        zaf::clipboard::MakePrivateFormatType(1);

public:
    void AddObject(std::shared_ptr<zaf::Object> object);

    const std::vector<std::shared_ptr<zaf::Object>>& Objects() const {
        return objects_;
    }

    zaf::clipboard::Medium SaveToMedium(const zaf::clipboard::Format& format) override;
    void LoadFromMedium(
        const zaf::clipboard::Format& format, 
        const zaf::clipboard::Medium& medium) override;

private:
    zaf::clipboard::Medium SaveToMediumAsText();
    zaf::clipboard::Medium SaveToMediumAsPrivateFormat();

    void LoadFromMediumAsText(const zaf::clipboard::Medium& medium);
    void LoadFromMediumAsPrivateFormat(const zaf::clipboard::Medium& medium);

private:
    std::vector<std::shared_ptr<zaf::Object>> objects_;
};

}