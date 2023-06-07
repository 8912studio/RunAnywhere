#include "main/clipboard_data.h"

namespace ra {

ClipboardData::ClipboardData(const std::vector<std::shared_ptr<zaf::Object>>& objects) :
    objects_(std::move(objects)) {

}


zaf::clipboard::Medium ClipboardData::SaveToMedium(const zaf::clipboard::Format& format) {
    return {};
}


void ClipboardData::LoadFromMedium(
    const zaf::clipboard::Format& format,
    const zaf::clipboard::Medium& medium) {


}

}