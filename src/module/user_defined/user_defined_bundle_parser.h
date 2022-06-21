#pragma once

#include <filesystem>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/error/error.h>
#include "module/user_defined/user_defined_bundle.h"

namespace ra::module::user_defined {

class UserDefinedBundleParser {
public:
    class ParseError : public zaf::Error {
    public:
        ParseError(int error_line_number, const std::string& error_line) :
            zaf::Error(zaf::BasicErrc::InvalidValue),
            error_line_number_(error_line_number), 
            error_line_(error_line) { 

        }

        int ErrorLineNumber() const {
            return error_line_number_;
        }

        const std::string& ErrorLine() const {
            return error_line_;
        }

    private:
        int error_line_number_{};
        std::string error_line_;
    };

public:
    UserDefinedBundleParser(const std::filesystem::path& bundle_path);

    UserDefinedBundleParser(const UserDefinedBundleParser&) = delete;
    UserDefinedBundleParser& operator=(const UserDefinedBundleParser&) = delete;

    //Throw zaf::Error if fail to open file.
    //Throw ParseError if fail to parse file content.
    std::shared_ptr<UserDefinedBundle> Parse();

private:
    std::filesystem::path bundle_path_;
};

}