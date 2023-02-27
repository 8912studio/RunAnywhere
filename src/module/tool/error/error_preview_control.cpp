#include "module/tool/error/error_preview_control.h"
#include <zaf/base/string/to_string.h>
#include <zaf/base/string/trim.h>
#include <zaf/object/type_definition.h>

namespace ra::module::tool::error {
namespace {

std::wstring GetErrorMessage(std::uint32_t error_code) {

    wchar_t* buffer{};

    auto result_length = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
        nullptr,
        error_code,
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

}

ZAF_DEFINE_TYPE(ErrorPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/error/error_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

ErrorPreviewControl::ErrorPreviewControl(
    const std::optional<ErrorCommandParseResult>& parse_result) 
    :
    parse_result_(parse_result) {

}


void ErrorPreviewControl::AfterParse() {

    __super::AfterParse();

    if (parse_result_) {
        ShowResult();
    }
}


void ErrorPreviewControl::ShowResult() const {

    errorCode->SetText(zaf::ToWideString(parse_result_->error_code, zaf::ToStringOptions().Base(16)));
    errorMessage->SetText(GetErrorMessage(parse_result_->error_code));
}


std::wstring ErrorPreviewControl::GetText() const {
    return errorMessage->Text();
}

}