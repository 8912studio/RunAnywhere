#pragma once

#include <curl/curl.h>
#include <zaf/base/error/runtime_error_base.h>

namespace ra::mod::ai::gpt {

class CURLError : public zaf::RuntimeErrorBase {
public:
    CURLError(CURLcode code, const std::string& message, const zaf::SourceLocation& location) : 
        RuntimeErrorBase(message, location),
        code_(code) {

    }

    CURLcode Code() const {
        return code_;
    }

private:
    CURLcode code_{};
};


class CURLMultiSocketError : public zaf::RuntimeErrorBase {
public:
    CURLMultiSocketError(CURLMcode code, const zaf::SourceLocation& location) :
        RuntimeErrorBase(location),
        code_(code) {

    }

    CURLMcode Code() const {
        return code_;
    }

private:
    CURLMcode code_{};
};


class HTTPError : public zaf::RuntimeErrorBase {
public:
    HTTPError(int status, const std::string& message, const zaf::SourceLocation& location) :
        RuntimeErrorBase(message, location),
        status_(status) {

    }

    int Status() const {
        return status_;
    }

private:
    int status_{};
};

}