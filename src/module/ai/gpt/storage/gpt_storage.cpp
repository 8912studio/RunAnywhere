#include "module/ai/gpt/storage/gpt_storage.h"
#include <format>
#include "utility/sql/table_schema.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

GPTStorage::GPTStorage(std::filesystem::path db_path) :
    context_(std::make_unique<StorageContext>(std::move(db_path))) {

}


void GPTStorage::InitializeDialogTable(Database& db) {

    std::call_once(dialog_table_once_flag_, [&db]() {
    
        db.CreateTable(TableSchema{
            .name = "Dialog",
            .columns = {
                {
                    .name = "ID", 
                    .data_type = DataType::Integer, 
                    .constraints = ColumnConstraints::PrimaryKey,
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
                    .name = "Subject",
                    .data_type = DataType::Text,
                },
            },
        });
    });
}


zaf::Observable<std::vector<DialogEntity>> GPTStorage::FetchAllDialogs() {

    return context_->Execute<std::vector<DialogEntity>>([this](Database& db) {

        InitializeDialogTable(db);

        std::vector<DialogEntity> result;

        auto sql = "select ID, CreateTime, UpdateTime, Subject from Dialog";
        auto statement = db.PrepareStatement(sql);
        while (statement.Step()) {

            DialogEntity dialog;
            dialog.id = statement.GetColumnInt64(0);
            dialog.create_time = statement.GetColumnInt64(1);
            dialog.update_time = statement.GetColumnInt64(2);
            dialog.subject = statement.GetColumnText(3);

            result.push_back(std::move(dialog));
        }

        return result;
    });
}


zaf::Observable<DialogID> GPTStorage::AddDialog(const DialogEntity& dialog) {

    return context_->Execute<DialogID>([this, dialog](Database& db) {
    
        InitializeDialogTable(db);

        auto sql = "insert into Dialog(ID, CreateTime, UpdateTime, Subject) values(?,?,?,?);";
        auto statement = db.PrepareStatement(sql);

        statement.BindParameter(1, dialog.id);
        statement.BindParameter(2, dialog.create_time);
        statement.BindParameter(3, dialog.update_time);
        statement.BindParameter(4, dialog.subject);
        statement.Step();

        return db.LastInsertRowID();
    });
}

}