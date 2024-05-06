#pragma once

#include <zaf/base/error/runtime_error.h>

namespace ra::mod::chat_gpt {

enum class LocalErrorCode {
    NoAPIKey,
    ChatOngoing,
};

class LocalError : public zaf::GeneralRuntimeError {
public:
    LocalError(LocalErrorCode code, const zaf::SourceSite& site) : 
        GeneralRuntimeError(site),
        code_(code) {

    }

    LocalErrorCode Code() const {
        return code_;
    }

private:
    LocalErrorCode code_{};
};

}