#include "module/tool/error/error_command.h"
#include <Windows.h>
#include <zaf/base/string/to_numeric.h>
#include <zaf/base/string/trim.h>
#include "utility/clipboard.h"

namespace ra::module::tool::error {

CommandBrief ErrorCommand::GetBrief() {
    return CommandBrief{
        L"err",
        L"Show error message"
    };
}


std::optional<ErrorCommandParseResult> ErrorCommand::Parse(
    const utility::CommandLine& command_line) {

    if (command_line.Arguments().empty()) {
        return std::nullopt;
    }

    ErrorCommandParseResult result;

    zaf::ToNumericOptions to_numeric_options;
    to_numeric_options.Base(10);

    bool is_succeeded = zaf::TryToNumeric<std::uint32_t>(
        command_line.Arguments().front(),
        result.error_code,
        to_numeric_options);

    if (is_succeeded) {
        return result;
    }

    to_numeric_options.Base(16);
    is_succeeded = zaf::TryToNumeric<std::uint32_t>(command_line.Arguments().front(),
        result.error_code,
        to_numeric_options);

    if (is_succeeded) {
        return result;
    }

    return std::nullopt;
}


ErrorCommand::ErrorCommand(const utility::CommandLine& command_line) :
    parse_result_(Parse(command_line)) {

}


std::wstring ErrorCommand::GetPreviewText() {

    if (!parse_result_) {
        return {};
    }

    wchar_t* buffer{};

    auto result_length = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        parse_result_->error_code,
        0,
        reinterpret_cast<LPWSTR>(&buffer),
        0,
        nullptr);

    std::wstring result;
    if (result_length > 0) {

        result.assign(buffer, result_length);
        zaf::Trim(result);

        LocalFree(buffer);
    }

    return result;
}


void ErrorCommand::Execute() {

    auto text = GetPreviewText();
    if (!text.empty()) {
        utility::SetStringToClipboard(text);
    }
}

}