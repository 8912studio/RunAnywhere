#include "module/user_defined/bundle_parser.h"
#include <fstream>
#include <string>
#include <zaf/base/error/system_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/object/boxing/boxing.h>
#include <zaf/object/enum_type.h>

namespace ra::mod::user_defined {
namespace {

bool TryToParseKeyword(const std::string& line, std::string& keyword) {

    if (line.front() != '[' || line.back() != ']') {
        return false;
    }

    keyword = zaf::ToTrimmed(line.substr(1, line.size() - 2));
    if (keyword.empty()) {
        return false;
    }

    return true;
}


bool TryToParseProperty(const std::string& line, std::string& key, std::string& value) {

    auto delimiter_index = line.find('=');
    if (delimiter_index == line.npos) {
        return false;
    }

    key = zaf::ToTrimmed(line.substr(0, delimiter_index));
    if (key.empty()) {
        return false;
    }

    value = zaf::ToTrimmed(line.substr(delimiter_index + 1));
    return true;
}


bool SetPropertyToEntry(
    Entry::Builder& builder, 
    const std::string& key,
    const std::string& value) {

    auto value_wstring = zaf::FromUTF8String(value);

    if (key == "Command") {
        builder.SetCommand(value_wstring);
    }
    else if (key == "Description") {
        builder.SetDescription(value_wstring);
    }
    else if (key == "WorkDir") {
        builder.SetWorkingDirectory(value_wstring);
    }
    else if (key == "ShowWindow") {

        auto enum_object = ShowWindowOptionEnum::EnumType()->FindValue(value_wstring);
        if (!enum_object) {
            return false;
        }

        auto enum_value = zaf::Unbox<ShowWindowOption>(*enum_object);
        builder.SetShowWindowOption(enum_value);
    }

    return true;
}

}

BundleParser::BundleParser(const std::filesystem::path& bundle_path) : 
    bundle_path_(bundle_path) {

}


std::shared_ptr<Bundle> BundleParser::Parse() {

    std::ifstream file_stream(bundle_path_, std::ios::in);
    if (!file_stream) {
        throw zaf::GeneralSystemError{
            std::make_error_code(std::io_errc::stream),
            ZAF_SOURCE_SITE()
        };
    }

    Bundle::Builder bundle_builder;

    BundleMeta::Builder meta_builder;
    meta_builder.SetBundleID(bundle_path_.stem());

    std::shared_ptr<BundleMeta> meta;

    std::unique_ptr<Entry::Builder> current_entry_builder;

    int line_number{};
    std::string line;
    while (std::getline(file_stream, line)) {

        line_number++;
        zaf::Trim(line);

        //Ignore empty line.
        if (line.empty()) {
            continue;
        }

        //Ignore comment line.
        if (line.front() == ';') {
            continue;
        }

        std::string keyword;
        if (TryToParseKeyword(line, keyword)) {

            //This is the first entry, finish meta context.
            if (!meta) {
                meta = meta_builder.Build();
                bundle_builder.SetMeta(meta);
            }

            //Already in entry context, finish it.
            if (current_entry_builder) {
                auto entry = current_entry_builder->Build();
                bundle_builder.AddEntry(entry);
            }

            //Start a new entry context.
            current_entry_builder = std::make_unique<Entry::Builder>();
            current_entry_builder->SetBundleMeta(meta);
            current_entry_builder->SetKeyword(zaf::FromUTF8String(keyword));
            continue;
        }

        std::string key;
        std::string value;
        if (TryToParseProperty(line, key, value)) {

            //Current in meta context, add to gloal property.
            if (!meta) {
                meta_builder.AddGlobalProperty(
                    zaf::FromUTF8String(key),
                    zaf::FromUTF8String(value));
            }
            //Current in entry context, add to entry property.
            else if (current_entry_builder) {

                bool is_valid_property = SetPropertyToEntry(*current_entry_builder, key, value);
                if (!is_valid_property) {
                    //Bad property.
                    throw ParseError(line_number, line, ZAF_SOURCE_SITE());
                }
            }
            continue;
        }

        //Bad line.
        throw ParseError(line_number, line, ZAF_SOURCE_SITE());
    }

    if (!meta) {
        bundle_builder.SetMeta(meta_builder.Build());
    }

    if (current_entry_builder) {
        bundle_builder.AddEntry(current_entry_builder->Build());
    }

    return bundle_builder.Build();
}

}