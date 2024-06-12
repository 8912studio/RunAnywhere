#pragma once

#include <zaf/clipboard/clipboard_data.h>
#include <zaf/object/object.h>

namespace ra::main::input {

class ClipboardData : public zaf::clipboard::ClipboardData {
public:
    static constexpr zaf::clipboard::FormatType PrivateFormatType = 
        zaf::clipboard::MakePrivateFormatType(1);

public:
    void AddObject(std::shared_ptr<zaf::Object> object);

    const std::vector<std::shared_ptr<zaf::Object>>& Objects() const {
        return objects_;
    }

    zaf::clipboard::Medium WriteToMedium(
        const zaf::clipboard::DataDescriptor& data_descriptor) const override;

    void ReadFromMedium(
        const zaf::clipboard::Medium& medium,
        const zaf::clipboard::DataDescriptor& data_descriptor) override;

private:
    zaf::clipboard::Medium WriteToMediumAsText() const;
    zaf::clipboard::Medium WriteToMediumAsPrivateFormat() const;

    void LoadFromMediumAsText(const zaf::clipboard::Medium& medium);
    void LoadFromMediumAsPrivateFormat(const zaf::clipboard::Medium& medium);

private:
    std::vector<std::shared_ptr<zaf::Object>> objects_;
};

}