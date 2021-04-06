#include "module/calculator/calculator_command.h"
#include <strsafe.h>
#include <sstream>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/to_string.h>

CalculatorCommand::CalculatorCommand(
    const calculator::EvaluateResult& evaluate_result,
    const calculator::Modifier& modifier)
    :
    evaluate_result_(evaluate_result),
    modifier_(modifier) {

}


std::wstring CalculatorCommand::GetPreviewText() {

    auto value_text = GetValueText();

    switch (modifier_.base) {
    case 2:
        value_text = L"0b" + value_text;
        break;
    case 8:
        value_text = L"0" + value_text;
        break;
    case 16:
        value_text = L"0x" + value_text;
        break;
    default:
        break;
    }

    return L"= " + value_text;
}


void CalculatorCommand::Execute() {

    BOOL is_succeeded = OpenClipboard(NULL);
    if (!is_succeeded) {
        return;
    }

    EmptyClipboard();
    SetStringToClipboard(GetValueText());
    CloseClipboard();
}


void CalculatorCommand::SetStringToClipboard(const std::wstring& string) {

    int memory_size = (string.length() + 1) * sizeof(wchar_t);

    HGLOBAL memory_handle = GlobalAlloc(GMEM_MOVEABLE, memory_size);
    if (memory_handle == NULL) {
        return;
    }

    DWORD error = CopyStringToMemory(string, memory_handle);
    if (error != ERROR_SUCCESS) {
        GlobalFree(memory_handle);
        return;
    }

    SetClipboardData(CF_UNICODETEXT, memory_handle);
    GlobalFree(memory_handle);
}


DWORD CalculatorCommand::CopyStringToMemory(const std::wstring& string, HGLOBAL memory_handle) {

    LPVOID memory = GlobalLock(memory_handle);
    if (memory == NULL) {
        return GetLastError();
    }

    HRESULT result = StringCchCopy(
        reinterpret_cast<wchar_t*>(memory),
        string.length() + 1,
        string.c_str());

    GlobalUnlock(memory_handle);

    if (FAILED(result)) {
        return ERROR_OUTOFMEMORY;
    }

    return ERROR_SUCCESS;
}


std::wstring CalculatorCommand::GetValueText() const {

    if (modifier_.base == 10) {
        return GetDecimalValueText();
    }
    else {
        return GetNonDecimalValueText();
    }
}


std::wstring CalculatorCommand::GetDecimalValueText() const {

    auto value = evaluate_result_.decimal_value;

    switch (modifier_.unit) {
    case calculator::NumberUnit::Tera:
        value /= 1024;
    case calculator::NumberUnit::Giga:
        value /= 1024;
    case calculator::NumberUnit::Mega:
        value /= 1024;
    case calculator::NumberUnit::Kilo:
        value /= 1024;
    default:
        break;
    }

    std::ostringstream stream;
    stream << std::setprecision(20) << value;
    return zaf::FromUtf8String(stream.str());
}


std::wstring CalculatorCommand::GetNonDecimalValueText() const {

    const auto& value = evaluate_result_.decimal_value;

    zaf::ToStringOptions to_string_options;
    to_string_options.Base(modifier_.base);

    std::wstring result;
    switch (modifier_.bit_length) {
    case 8:
        result = zaf::ToWideString(value.convert_to<std::int8_t>(), to_string_options);
        break;

    case 16:
        result = zaf::ToWideString(value.convert_to<std::int16_t>(), to_string_options);
        break;

    case 64:
        result = zaf::ToWideString(value.convert_to<std::int64_t>(), to_string_options);
        break;

    default:
        result = zaf::ToWideString(value.convert_to<std::int32_t>(), to_string_options);
        break;
    }

    if (modifier_.use_upper_case) {
        zaf::Uppercase(result);
    }

    return result;
}