#pragma once

#include <zaf/object/object.h>
#include "module/ai/gpt/storage/dialog_entity.h"

namespace ra::mod::ai::gpt {

class DialogItemData : public zaf::Object {
public:
    DialogItemData(DialogEntity entity, std::uint64_t transient_id) : 
        entity_(std::move(entity)), 
        transient_id_(transient_id) {

    }

    const DialogEntity& Entity() const {
        return entity_;
    }

    std::uint64_t TransientID() const {
        return transient_id_;
    }

private:
    DialogEntity entity_;
    std::uint64_t transient_id_{};
};

}