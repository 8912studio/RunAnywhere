#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/object/object.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

class Dialog : public zaf::Object, zaf::NonCopyableNonMovable {
public:
    Dialog(DialogTransientID transient_id, DialogEntity entity);
    explicit Dialog(DialogEntity entity);

    DialogID ID() const;

    const DialogEntity& Entity() const {
        return entity_;
    }

private:
    DialogTransientID transient_id_;
    DialogEntity entity_;
};

}