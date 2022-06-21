#include "module/user_defined/user_defined_bundle_parser.h"
#include <fstream>
#include <string>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>

namespace ra::module::user_defined {
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


void SetPropertyToEntry(
    UserDefinedEntry::Builder& builder, 
    const std::string& key,
    const std::string& value) {

    auto value_wstring = zaf::FromUtf8String(value);

    if (key == "Command") {
        builder.SetCommand(value_wstring);
    }
    else if (key == "Description") {
        builder.SetDescription(value_wstring);
    }
}

}

UserDefinedBundleParser::UserDefinedBundleParser(const std::filesystem::path& bundle_path) : 
    bundle_path_(bundle_path) {

}


std::shared_ptr<UserDefinedBundle> UserDefinedBundleParser::Parse() {

    std::ifstream file_stream(bundle_path_, std::ios::in);
    if (!file_stream) {
        throw zaf::Error(std::make_error_code(std::io_errc::stream));
    }

    UserDefinedBundle::Builder bundle_builder;

    UserDefinedBundleMeta::Builder meta_builder;
    meta_builder.SetFilePath(bundle_path_);

    std::shared_ptr<UserDefinedBundleMeta> meta;

    std::unique_ptr<UserDefinedEntry::Builder> current_entry_builder;

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
            current_entry_builder = std::make_unique<UserDefinedEntry::Builder>();
            current_entry_builder->SetBundleMeta(meta);
            current_entry_builder->SetKeyword(zaf::FromUtf8String(keyword));
            continue;
        }

        std::string key;
        std::string value;
        if (TryToParseProperty(line, key, value)) {

            //Current in meta context, add to gloal property.
            if (!meta) {
                //TODO:
            }
            //Current in entry context, add to entry property.
            else if (current_entry_builder) {
                SetPropertyToEntry(*current_entry_builder, key, value);
            }
            continue;
        }

        //Bad line.
        throw ParseError(line_number, line);
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