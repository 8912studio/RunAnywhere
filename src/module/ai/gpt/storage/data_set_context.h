#pragma once

#include <zaf/base/non_copyable.h>
#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

template<typename E>
class DataSetContext {
public:
    explicit DataSetContext();

private:
    std::shared_ptr<StorageContext> context_;
};

}