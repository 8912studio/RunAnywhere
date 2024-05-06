#pragma once

#include <curl/curl.h>
#include <zaf/base/error/runtime_error.h>

namespace ra::mod::chat_gpt::comm {

class CURLError : public zaf::GeneralRuntimeError {
public:
    CURLError(CURLcode code, const std::string& message, const zaf::SourceSite& site) : 
        GeneralRuntimeError(message, site),
        code_(code) {

    }

    CURLcode Code() const {
        return code_;
    }

private:
    CURLcode code_{};
};


class CURLMultiSocketError : public zaf::GeneralRuntimeError {
public:
    CURLMultiSocketError(CURLMcode code, const zaf::SourceSite& site) :
        GeneralRuntimeError(site),
        code_(code) {

    }

    CURLMcode Code() const {
        return code_;
    }

private:
    CURLMcode code_{};
};


class HTTPError : public zaf::GeneralRuntimeError {
public:
    HTTPError(int status, const std::string& message, const zaf::SourceSite& site) : 
        GeneralRuntimeError(message, site),
        status_(status) {

    }

    int Status() const {
        return status_;
    }

private:
    int status_{};
};

}