#pragma once

#include "module/ai/gpt/storage/round_entity.h"
#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

class RoundStorage {
public:
    explicit RoundStorage(std::shared_ptr<StorageContext> context);

    zaf::Observable<std::vector<RoundEntity>> FetchAllRoundsInDialog(DialogID dialog_id);

    zaf::Observable<RoundPermanentID> AddRound(const RoundEntity& round_entity);
    zaf::Observable<RoundPermanentID> UpdateRound(const RoundEntity& round_entity);
    zaf::Observable<RoundPermanentID> DeleteRound(RoundPermanentID permanent_id);

private:
    void InitializeRoundTable(utility::sql::Database& db);

private:
    std::shared_ptr<StorageContext> context_;
    std::once_flag round_table_once_flag_;
};

}