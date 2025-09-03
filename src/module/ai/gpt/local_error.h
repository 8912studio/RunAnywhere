#pragma once

#include <zaf/base/error/runtime_error_base.h>

namespace ra::mod::ai::gpt {

enum class LocalErrorCode {
    NoAPIKey,
    ChatOngoing,
};

class LocalError : public zaf::RuntimeErrorBase {
public:
    LocalError(LocalErrorCode code, const zaf::SourceLocation& location) : 
        RuntimeErrorBase(location),
        code_(code) {

    }

    LocalErrorCode Code() const {
        return code_;
    }

private:
    LocalErrorCode code_{};
};

}