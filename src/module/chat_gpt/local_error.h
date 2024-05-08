#pragma once

#include <zaf/base/error/base_runtime_error.h>

namespace ra::mod::chat_gpt {

enum class LocalErrorCode {
    NoAPIKey,
    ChatOngoing,
};

class LocalError : public zaf::BaseRuntimeError {
public:
    LocalError(LocalErrorCode code, const zaf::SourceLocation& location) : 
        BaseRuntimeError(location),
        code_(code) {

    }

    LocalErrorCode Code() const {
        return code_;
    }

private:
    LocalErrorCode code_{};
};

}