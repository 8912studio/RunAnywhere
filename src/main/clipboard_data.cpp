#include "main/clipboard_data.h"
#include <zaf/base/as.h>
#include <zaf/base/base64.h>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/string/join.h>
#include <zaf/object/boxing/string.h>
#include "main/text_block_data.h"

namespace ra {

void ClipboardData::AddObject(std::shared_ptr<zaf::Object> object) {
    objects_.push_back(std::move(object));
}


zaf::clipboard::Medium ClipboardData::SaveToMedium(const zaf::clipboard::Format& format) {

    if (format.Type() == zaf::clipboard::FormatType::Text &&
        format.MediumType() == zaf::clipboard::MediumType::GlobalMem) {

        return SaveTextToMedium();
    }

    ZAF_THROW_ERRC(zaf::BasicErrc::Unsupported);
}


zaf::clipboard::Medium ClipboardData::SaveTextToMedium() {

    auto all_text = zaf::JoinAsWideString(objects_, L" ", [](const auto& object) {
        
        if (auto string = zaf::As<zaf::WideString>(object)) {
            return string->Value();
        }

        if (auto text_block_data = zaf::As<TextBlockData>(object)) {
            return text_block_data->Text();
        }

        return std::wstring{};
    });

    auto global_mem = zaf::GlobalMem::FromString(all_text, zaf::GlobalMemFlags::Movable);
    return zaf::clipboard::Medium::FromGlobalMem(std::move(global_mem));
}


void ClipboardData::LoadFromMedium(
    const zaf::clipboard::Format& format,
    const zaf::clipboard::Medium& medium) {


}

}