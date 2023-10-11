#pragma once

#include <system_error>

namespace ra::mod::chat_gpt::comm {

inline const std::error_category& CURLErrorCategory() {

    class CURLErrorCategory : public std::error_category {
    public:
        const char* name() const noexcept override {
            return "CURLcode";
        }

        std::string message(int condition) const override {
            return std::string();
        }
    };

    static CURLErrorCategory category;
    return category;
}


inline const std::error_category& HTTPErrorCategory() {

    class HTTPErrorCategory : public std::error_category {
    public:
        const char* name() const noexcept override {
            return "HTTP";
        }

        std::string message(int condition) const override {
            return std::string();
        }
    };

    static HTTPErrorCategory category;
    return category;
}

}