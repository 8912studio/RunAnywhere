#include "module/ai/gpt/storage/round_storage.h"

namespace ra::mod::ai::gpt {

RoundStorage::RoundStorage(std::shared_ptr<ScheduledStorageContext> context) :
    context_(std::move(context)) {

}


zaf::Observable<std::vector<RoundEntity>> RoundStorage::FetchAllRoundsInDialog(
    std::uint64_t dialog_id) {

    return context_->Execute<std::vector<RoundEntity>>([this, dialog_id](StorageContext& context) {

        constexpr auto& round_table = sqt::Table<RoundEntity>;
        constexpr auto selecter = 
            sqt::DataContext<RoundEntity>::MakeSelecter().Where(round_table.DialogID == sqt::_);

        auto executor = context.RoundDataContext().Prepare(selecter);
        executor.BeginBindings().Bind(dialog_id);
        return executor.Execute().ToVector();
    });
}


zaf::Observable<std::uint64_t> RoundStorage::AddRound(const RoundEntity& round_entity) {

    return context_->Execute<std::uint64_t>([this, round_entity](StorageContext& context) {

        return context.RoundDataContext().AutoIncInsert(round_entity);
    });
}


zaf::Observable<std::uint64_t> RoundStorage::UpdateRound(const RoundEntity& round_entity) {

    return context_->Execute<std::uint64_t>([this, round_entity](StorageContext& context) {
    
        constexpr auto& round_table = sqt::Table<RoundEntity>;
        constexpr auto updater = 
            sqt::DataContext<RoundEntity>::MakeUpdater(
                round_table.CreateTime = sqt::_,
                round_table.UpdateTime = sqt::_,
                round_table.Question = sqt::_,
                round_table.Response = sqt::_
            )
            .Where(round_table.ID == sqt::_);

        auto executor = context.RoundDataContext().Prepare(updater);
        executor.BeginBindings().BindAllFromEntity(round_entity);
        executor.Execute();
        return round_entity.id;
    });
}


zaf::Observable<std::uint64_t> RoundStorage::DeleteRound(std::uint64_t permanent_id) {

    return context_->Execute<std::uint64_t>([this, permanent_id](StorageContext& context) {
    
        context.RoundDataContext().Delete(permanent_id);
        return permanent_id;
    });
}


zaf::Observable<zaf::None> RoundStorage::DeleteAllRoundsInDialog(std::uint64_t dialog_id) {

    return context_->Execute<zaf::None>([this, dialog_id](StorageContext& context) {

        constexpr auto& round_table = sqt::Table<RoundEntity>;
        constexpr auto deleter = 
            sqt::DataContext<RoundEntity>::MakeDeleter().Where(round_table.DialogID == sqt::_);

        auto executor = context.RoundDataContext().Prepare(deleter);
        executor.BeginBindings().Bind(dialog_id);
        executor.Execute();

        return zaf::None{};
    });
}

}