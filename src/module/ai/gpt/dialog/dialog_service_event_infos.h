#pragma once

#include <memory>
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

struct DialogCreatedInfo {
    std::shared_ptr<Dialog> dialog;
};

struct DialogPersistedInfo {
    DialogTransientID transient_id;
    DialogPermanentID permanent_id;
};

struct DialogUpdatedInfo {
    std::shared_ptr<Dialog> dialog;
};

struct RoundCreatedInfo {
    DialogID dialog_id;
    std::shared_ptr<Round> round;
};

struct RoundPersistedInfo {
    DialogID dialog_id;
    RoundTransientID transient_id;
    RoundPermanentID permanent_id;
};

}