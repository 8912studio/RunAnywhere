#include "module/common/copy_executor.h"
#include "utility/clipboard.h"

namespace ra::mod {

std::shared_ptr<CopyExecutor> CopyExecutor::TryCreate(std::wstring text) {

    if (text.empty()) {
        return nullptr;
    }

    return std::shared_ptr<CopyExecutor>(new CopyExecutor{ std::move(text) });
}


CopyExecutor::CopyExecutor(std::wstring text) : text_(std::move(text)) {

}


void CopyExecutor::Execute() {

    utility::SetStringToClipboard(text_);
}

}