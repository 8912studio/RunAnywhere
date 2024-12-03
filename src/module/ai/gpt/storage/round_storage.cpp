#include "module/ai/gpt/storage/round_storage.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

RoundStorage::RoundStorage(std::shared_ptr<ScheduledStorageContext> context) :
    context_(std::move(context)) {

}


zaf::Observable<std::vector<RoundEntity>> RoundStorage::FetchAllRoundsInDialog(
    std::uint64_t dialog_id) {

    return context_->Execute<std::vector<RoundEntity>>([this, dialog_id](StorageContext& context) {

        auto& db = context.DB();

        auto sql = 
            "select ID, DialogID, CreateTime, UpdateTime, "
            "Question, Response from Round where DialogID = ?;";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, dialog_id);

        std::vector<RoundEntity> result;
        while (statement.Step()) {

            RoundEntity entity;
            entity.id = statement.GetColumnInt64(0);
            entity.dialog_id = statement.GetColumnInt64(1);
            entity.create_time = statement.GetColumnInt64(2);
            entity.update_time = statement.GetColumnInt64(3);
            entity.question = statement.GetColumnText(4);
            entity.response = statement.GetColumnText(5);

            result.push_back(std::move(entity));
        }

        return result;
    });
}


zaf::Observable<std::uint64_t> RoundStorage::AddRound(const RoundEntity& round_entity) {

    return context_->Execute<std::uint64_t>([this, round_entity](StorageContext& context) {

        return context.RoundDataSet().InsertWithAutoincrement(round_entity);
    });
}


zaf::Observable<std::uint64_t> RoundStorage::UpdateRound(const RoundEntity& round_entity) {

    return context_->Execute<std::uint64_t>([this, round_entity](StorageContext& context) {
    
        auto& db = context.DB();

        auto sql =
            "update Round set CreateTime = ?, UpdateTime = ?, Question = ?, Response = ? "
            "where ID = ?";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, round_entity.create_time);
        statement.BindParameter(2, round_entity.update_time);
        statement.BindParameter(3, round_entity.question);
        statement.BindParameter(4, round_entity.response);
        statement.BindParameter(5, round_entity.id);

        statement.Step();
        return round_entity.id;
    });
}


zaf::Observable<std::uint64_t> RoundStorage::DeleteRound(std::uint64_t permanent_id) {

    return context_->Execute<std::uint64_t>([this, permanent_id](StorageContext& context) {
    
        context.RoundDataSet().Delete(permanent_id);
        return permanent_id;
    });
}


zaf::Observable<zaf::None> RoundStorage::DeleteAllRoundsInDialog(std::uint64_t dialog_id) {

    return context_->Execute<zaf::None>([this, dialog_id](StorageContext& context) {
    
        auto sql = "delete from Round where DialogID = ?;";

        auto& db = context.DB();
        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, dialog_id);

        statement.Step();
        return zaf::None{};
    });
}

}


