#pragma once

#include <curl/curl.h>
#include <zaf/base/error/base_runtime_error.h>

namespace ra::mod::ai::gpt {

class CURLError : public zaf::BaseRuntimeError {
public:
    CURLError(CURLcode code, const std::string& message, const zaf::SourceLocation& location) : 
        BaseRuntimeError(message, location),
        code_(code) {

    }

    CURLcode Code() const {
        return code_;
    }

private:
    CURLcode code_{};
};


class CURLMultiSocketError : public zaf::BaseRuntimeError {
public:
    CURLMultiSocketError(CURLMcode code, const zaf::SourceLocation& location) :
        BaseRuntimeError(location),
        code_(code) {

    }

    CURLMcode Code() const {
        return code_;
    }

private:
    CURLMcode code_{};
};


class HTTPError : public zaf::BaseRuntimeError {
public:
    HTTPError(int status, const std::string& message, const zaf::SourceLocation& location) :
        BaseRuntimeError(message, location),
        status_(status) {

    }

    int Status() const {
        return status_;
    }

private:
    int status_{};
};

}