#pragma once

#include <string>
#include <variant>
#include <zaf/base/error/error.h>

namespace ra::mod::chat_gpt {

class AnswerResult {
public:
    explicit AnswerResult(std::variant<std::wstring, zaf::Error> variant) :
        variant_(std::move(variant)) {

    }

    const std::wstring* Answer() const {
        return std::get_if<std::wstring>(&variant_);
    }

    const zaf::Error* Error() const {
        return std::get_if<zaf::Error>(&variant_);
    }

private:
    std::variant<std::wstring, zaf::Error> variant_;
};

}