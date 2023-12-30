#pragma once

#include <system_error>

namespace ra::mod::chat_gpt {

enum class LocalErrc {
    NoAPIKey,
    ChatOngoing,
};

const std::error_category& LocalErrorCategory();

inline std::error_code make_error_code(LocalErrc errc) noexcept {
    return std::error_code{ static_cast<int>(errc), LocalErrorCategory() };
}

}


namespace std {

template<>
struct is_error_code_enum<ra::mod::chat_gpt::LocalErrc> : public true_type { };

}