#include "module/calculator/calculator_command.h"
#include <strsafe.h>
#include <zaf/creation.h>
#include "module/calculator/preview/calculator_preview_control.h"
#include "module/calculator/result_text_builder.h"

CalculatorCommand::CalculatorCommand(
    const calculator::EvaluateResult& evaluate_result,
    const calculator::Modifier& modifier)
    :
    evaluate_result_(evaluate_result),
    modifier_(modifier) {

}


std::shared_ptr<CommandPreviewControl> CalculatorCommand::GetPreviewControl() {

    auto control = zaf::Create<CalculatorPreviewControl>();
    control->SetResult(evaluate_result_, modifier_);
    return control;
}


void CalculatorCommand::Execute() {

    BOOL is_succeeded = OpenClipboard(NULL);
    if (!is_succeeded) {
        return;
    }

    ResultTextBuilder text_builder(evaluate_result_, modifier_);
    auto result_text = text_builder.Build();

    EmptyClipboard();
    SetStringToClipboard(result_text.GetCompleteText());
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