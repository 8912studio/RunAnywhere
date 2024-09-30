#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/object/object.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

class Dialog : public zaf::Object, zaf::NonCopyableNonMovable {
public:
    Dialog(const DialogID& id, DialogEntity entity);

    const DialogID& ID() const {
        return id_;
    }

    const DialogEntity& Entity() const {
        return entity_;
    }

private:
    DialogID id_;
    DialogEntity entity_;
};

using DialogList = std::vector<std::shared_ptr<Dialog>>;

}