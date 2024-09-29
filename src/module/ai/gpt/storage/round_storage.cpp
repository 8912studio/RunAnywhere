#include "module/ai/gpt/storage/round_storage.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

RoundStorage::RoundStorage(std::shared_ptr<StorageContext> context) : 
    context_(std::move(context)) {

}


void RoundStorage::InitializeRoundTable(utility::sql::Database& db) {

    std::call_once(round_table_once_flag_, [&db]() {

        db.CreateTable(TableSchema{
            .name = "Round",
            .columns = {
                {
                    .name = "PermanentID",
                    .data_type = DataType::Integer,
                    .constraints = ColumnConstraints::PrimaryKey,
                },
                {
                    .name = "TransientID",
                    .data_type = DataType::Integer,
                },
                {
                    .name = "DialogPermanentID",
                    .data_type = DataType::Integer,
                },
                {
                    .name = "CreateTime",
                    .data_type = DataType::Integer,
                },
                {
                    .name = "UpdateTime",
                    .data_type = DataType::Integer,
                },
                {
                    .name = "Question",
                    .data_type = DataType::Text,
                },
                {
                    .name = "Response",
                    .data_type = DataType::Text,
                },
            },
        });
    });
}


zaf::Observable<std::vector<RoundEntity>> RoundStorage::FetchAllRoundsInDialog(
    DialogID dialog_id) {

    return context_->Execute<std::vector<RoundEntity>>([this, dialog_id](Database& db) {

        InitializeRoundTable(db);

        auto sql = 
            "select PermanentID, TransientID, DialogPermanentID, CreateTime, UpdateTime, "
            "Question, Response from Round where DialogPermanentID = ?;";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, dialog_id);

        std::vector<RoundEntity> result;
        while (statement.Step()) {

            RoundEntity entity;
            entity.permanent_id = statement.GetColumnInt64(0);
            entity.transient_id = statement.GetColumnInt64(1);
            entity.dialog_id = statement.GetColumnInt64(2);
            entity.create_time = statement.GetColumnInt64(3);
            entity.update_time = statement.GetColumnInt64(4);
            entity.question = statement.GetColumnText(5);
            entity.response = statement.GetColumnText(6);

            result.push_back(std::move(entity));
        }

        return result;
    });
}


zaf::Observable<RoundPermanentID> RoundStorage::AddRound(const RoundEntity& round_entity) {

    return context_->Execute<RoundPermanentID>([this, round_entity](Database& db) {

        InitializeRoundTable(db);

        auto sql =
            "insert into Round"
            "(TransientID, DialogPermanentID, CreateTime, UpdateTime, Question, Response) values"
            "(?, ?, ?, ?, ?, ?);";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, round_entity.transient_id);
        statement.BindParameter(2, round_entity.dialog_id);
        statement.BindParameter(3, round_entity.create_time);
        statement.BindParameter(4, round_entity.update_time);
        statement.BindParameter(5, round_entity.question);
        statement.BindParameter(6, round_entity.response);

        statement.Step();
        return db.LastInsertRowID();
    });
}


zaf::Observable<RoundPermanentID> RoundStorage::UpdateRound(const RoundEntity& round_entity) {

    return context_->Execute<RoundPermanentID>([this, round_entity](Database& db) {
    
        InitializeRoundTable(db);

        auto sql =
            "update Round set CreateTime = ?, UpdateTime = ?, Question = ?, Response = ? "
            "where ID = ?";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, round_entity.create_time);
        statement.BindParameter(2, round_entity.update_time);
        statement.BindParameter(3, round_entity.question);
        statement.BindParameter(4, round_entity.response);
        statement.BindParameter(5, round_entity.permanent_id);

        statement.Step();
        return round_entity.permanent_id;
    });
}


zaf::Observable<RoundPermanentID> RoundStorage::DeleteRound(RoundPermanentID permanent_id) {

    return context_->Execute<RoundPermanentID>([this, permanent_id](Database& db) {
    
        InitializeRoundTable(db);

        auto sql = "delete from Round where PermanentID = ?;";

        auto statement = db.PrepareStatement(sql);
        statement.BindParameter(1, permanent_id);

        statement.Step();
        return permanent_id;
    });
}

}


