#include "module/ai/gpt/dialog/dialog.h"

namespace ra::mod::ai::gpt {

Dialog::Dialog(const DialogID& id, DialogEntity entity) : id_(id), entity_(std::move(entity)) {

}

}