#include "module/ai/gpt/dialog/dialog.h"
#include <zaf/base/error/contract_error.h>

namespace ra::mod::ai::gpt {

Dialog::Dialog(const DialogID& id, DialogEntity entity) : id_(id), entity_(std::move(entity)) {

    if (auto permanent_id = id_.PermanentID()) {
        ZAF_EXPECT(permanent_id->Value() == entity_.id);
    }
    else {
        ZAF_EXPECT(entity_.id == 0);
    }
}

}