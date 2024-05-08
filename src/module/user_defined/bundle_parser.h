#pragma once

#include <filesystem>
#include <zaf/base/error/base_runtime_error.h>
#include "module/user_defined/bundle.h"

namespace ra::mod::user_defined {

class BundleParser {
public:
    class ParseError : public zaf::BaseRuntimeError {
    public:
        ParseError(
            int error_line_number, 
            const std::string& error_line,
            const zaf::SourceLocation& location)
            :
            BaseRuntimeError(location),
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
    BundleParser(const std::filesystem::path& bundle_path);

    BundleParser(const BundleParser&) = delete;
    BundleParser& operator=(const BundleParser&) = delete;

    //Throw std::system_error if fail to open file.
    //Throw ParseError if fail to parse file content.
    std::shared_ptr<Bundle> Parse();

private:
    std::filesystem::path bundle_path_;
};

}