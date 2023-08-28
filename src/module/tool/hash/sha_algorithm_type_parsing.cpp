#include "module/tool/hash/sha_algorithm_type_parsing.h"

namespace ra::mod::tool::hash {
namespace {

std::optional<SHAAlgorithmType> ParseSingleArgument(const utility::CommandLinePiece& argument) {

    if (argument.Type() != utility::CommandLinePieceType::NormalText) {
        return std::nullopt;
    }

    std::wstring_view content = argument.Content();
    if (content.empty()) {
        return std::nullopt;
    }

    if (content.front() != L'/') {
        return std::nullopt;
    }

    auto value = content.substr(1);
    if (value == L"1") {
        return SHAAlgorithmType::SHA1;
    }
    if (value == L"256") {
        return SHAAlgorithmType::SHA256;
    }
    if (value == L"384") {
        return SHAAlgorithmType::SHA384;
    }
    if (value == L"512") {
        return SHAAlgorithmType::SHA512;
    }
    return std::nullopt;
}

}

std::optional<SHAAlgorithmType> ParseSHAAlgorithmType(const utility::CommandLine& command_line) {

    std::optional<SHAAlgorithmType> result;

    for (const auto& each_piece : command_line.Arguments()) {

        auto algorithm_type = ParseSingleArgument(each_piece);
        if (algorithm_type) {
            result = algorithm_type;
        }
    }

    return result;
}

}