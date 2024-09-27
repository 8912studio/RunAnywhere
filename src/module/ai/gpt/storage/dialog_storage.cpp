#include "module/ai/gpt/storage/dialog_storage.h"
#include "utility/sql/table_schema.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

DialogStorage::DialogStorage(std::shared_ptr<StorageContext> context) : 
    context_(std::move(context)) {

}


void DialogStorage::InitializeDialogTable(utility::sql::Database& db) {

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


zaf::Observable<std::vector<DialogEntity>> DialogStorage::FetchAllDialogs() {

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


zaf::Observable<DialogID> DialogStorage::AddDialog(const DialogEntity& dialog) {

    return context_->Execute<DialogID>([this, dialog](Database& db) {

        InitializeDialogTable(db);

        auto sql = "insert into Dialog(CreateTime, UpdateTime, Subject) values(?,?,?);";
        auto statement = db.PrepareStatement(sql);

        statement.BindParameter(1, dialog.create_time);
        statement.BindParameter(2, dialog.update_time);
        statement.BindParameter(3, dialog.subject);
        statement.Step();

        return db.LastInsertRowID();
    });
}


zaf::Observable<DialogID> DialogStorage::UpdateDialog(const DialogEntity& dialog) {

    return context_->Execute<DialogID>([this, dialog](Database& db) {
    
        InitializeDialogTable(db);

        auto sql = "update Dialog set CreateTime=?, UpdateTime=?, Subject=? where ID = ?;";
        auto statement = db.PrepareStatement(sql);

        statement.BindParameter(1, dialog.create_time);
        statement.BindParameter(2, dialog.update_time);
        statement.BindParameter(3, dialog.subject);
        statement.BindParameter(4, dialog.id);
        statement.Step();

        return dialog.id;
    });
}

}