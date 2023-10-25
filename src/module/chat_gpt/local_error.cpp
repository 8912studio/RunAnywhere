#include "module/chat_gpt/local_error.h"

namespace ra::mod::chat_gpt {
namespace {

class LocalErrorCategory : public std::error_category {
public:
    const char* name() const noexcept override {
        return "ChatGPTLocalError";
    }

    std::string message(int condition) const override {
        return std::string{};
    }
};

}

const std::error_category& LocalCategory() {
    static LocalErrorCategory category;
    return category;
}

}