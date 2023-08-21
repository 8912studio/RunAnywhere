#include "main/input/clipboard_data.h"
#include <boost/json.hpp>
#include <zaf/base/as.h>
#include <zaf/base/base64.h>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/join.h>
#include <zaf/clipboard/clipboard.h>
#include <zaf/object/boxing/boxing.h>
#include <zaf/object/boxing/string.h>
#include <zaf/object/type_definition.h>
#include "main/input/text_block_data.h"

namespace ra::main::input {
namespace {

constexpr const char* JSONItemsName = "items";
constexpr const char* JSONTypeName = "type";
constexpr const char* JSONValueName = "value";

enum class JSONItemType {
    String,
    TextBlock,
};

std::string ToJSONValue(std::wstring_view string) {
    auto utf8 = zaf::ToUTF8String(string);
    return zaf::Base64Encode(utf8.data(), utf8.length());
}

std::wstring FromJSONValue(std::string_view string) {

    auto decoded = zaf::Base64Decode(string);

    return zaf::FromUTF8String(std::string_view{
        reinterpret_cast<const char*>(decoded.data()), 
        decoded.size() 
    });
}

}

ZAF_DEFINE_TYPE(ClipboardData)
ZAF_DEFINE_TYPE_END;

void ClipboardData::RegisterToClipboard() {
    zaf::clipboard::Clipboard::RegisterClipboardData(PrivateFormatType, ClipboardData::Type);
}


void ClipboardData::AddObject(std::shared_ptr<zaf::Object> object) {
    objects_.push_back(std::move(object));
}


zaf::clipboard::Medium ClipboardData::SaveToMedium(const zaf::clipboard::Format& format) {

    if (format.MediumType() == zaf::clipboard::MediumType::GlobalMem) {

        if (format.Type() == zaf::clipboard::FormatType::Text) {
            return SaveToMediumAsText();
        }

        if (format.Type() == PrivateFormatType) {
            return SaveToMediumAsPrivateFormat();
        }
    }

    ZAF_THROW_ERRC(zaf::BasicErrc::Unsupported);
}


zaf::clipboard::Medium ClipboardData::SaveToMediumAsText() {

    auto all_text = zaf::JoinAsWideString(objects_, L" ", [](const auto& object) {
        
        if (auto string = zaf::As<zaf::WideString>(object)) {
            return string->Value();
        }

        if (auto text_block_data = zaf::As<TextBlockData>(object)) {
            return text_block_data->Text();
        }

        return std::wstring{};
    });

    return zaf::clipboard::Medium::FromString(all_text);
}


zaf::clipboard::Medium ClipboardData::SaveToMediumAsPrivateFormat() {

    boost::json::array items;

    for (const auto& each_object : objects_) {

        if (auto string = zaf::As<zaf::WideString>(each_object)) {

            boost::json::object item;
            item[JSONTypeName] = static_cast<int>(JSONItemType::String);
            item[JSONValueName] = ToJSONValue(string->Value());
            items.push_back(std::move(item));
        }
        else if (auto text_block_data = zaf::As<TextBlockData>(each_object)) {

            boost::json::object item;
            item[JSONTypeName] = static_cast<int>(JSONItemType::TextBlock);
            item[JSONValueName] = ToJSONValue(text_block_data->Text());
            items.push_back(std::move(item));
        }
    }

    boost::json::object root_object;
    root_object[JSONItemsName] = std::move(items);

    auto json_text = boost::json::serialize(root_object);

    return zaf::clipboard::Medium::FromString(zaf::FromUTF8String(json_text));
}


void ClipboardData::LoadFromMedium(
    const zaf::clipboard::Format& format,
    const zaf::clipboard::Medium& medium) {

    if (medium.Type() == zaf::clipboard::MediumType::GlobalMem) {
    
        if (format.Type() == zaf::clipboard::FormatType::Text) {
            LoadFromMediumAsText(medium);
            return;
        }
        else if (format.Type() == PrivateFormatType) {
            LoadFromMediumAsPrivateFormat(medium);
            return;
        }
    }

    ZAF_THROW_ERRC(zaf::BasicErrc::Unsupported);
}


void ClipboardData::LoadFromMediumAsText(const zaf::clipboard::Medium& medium) {

    objects_.clear();

    try {
        auto string = medium.ToString();
        objects_.push_back(zaf::Box(string));
    }
    catch (...) {

    }
}


void ClipboardData::LoadFromMediumAsPrivateFormat(const zaf::clipboard::Medium& medium) {

    objects_.clear();

    try {

        auto string = medium.ToString();
        auto root_object = boost::json::parse(zaf::ToUTF8String(string)).as_object();

        const auto& items = root_object[JSONItemsName].as_array();
        for (const auto& each_item : items) {

            const auto& item = each_item.as_object();
            auto item_type = static_cast<JSONItemType>(item.at(JSONTypeName).get_int64());
            auto text = FromJSONValue(item.at(JSONValueName).get_string());

            switch (item_type) {
            case JSONItemType::String: 
                objects_.push_back(zaf::Box(text));
                break;
            case JSONItemType::TextBlock: 
                objects_.push_back(std::make_shared<TextBlockData>(std::move(text)));
                break;
            default:
                break;
            }
        }
    }
    catch (...) {
        objects_.clear();
    }
}

}