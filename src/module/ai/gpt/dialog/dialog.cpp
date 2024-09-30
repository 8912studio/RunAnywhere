#include "module/ai/gpt/dialog/dialog.h"
#include <zaf/base/error/contract_error.h>

namespace ra::mod::ai::gpt {

Dialog::Dialog(DialogTransientID transient_id, DialogEntity entity) :
    transient_id_(transient_id),
    entity_(std::move(entity)) {

    ZAF_EXPECT(entity_.id == 0);
}


Dialog::Dialog(DialogEntity entity) : entity_(std::move(entity)) {

    ZAF_EXPECT(entity_.id != 0);
}


DialogID Dialog::ID() const {

    if (transient_id_.Value() != 0) {
        return DialogID{ transient_id_ };
    }

    return DialogID{ DialogPermanentID{ entity_.id } };
}

}