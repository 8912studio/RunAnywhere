#include <gtest/gtest.h>
#include <zaf/base/as.h>
#include "module/ai/gpt/dialog/list/dialog_data_source.h"

using namespace ra::mod::ai::gpt;
using namespace zaf;

namespace {

std::shared_ptr<Dialog> MakeDialog(std::uint64_t id) {

    DialogEntity entity;
    entity.update_time = id;

    return std::make_shared<Dialog>(
        DialogID{ DialogTransientID{ id } }, 
        std::move(entity));
}

}

TEST(DialogDataSourceTest, AddDialogs_AddToEmpty) {

    DialogDataSource data_source;
    data_source.AddDialogs({
        MakeDialog(10),
        MakeDialog(9),
        MakeDialog(14),
        MakeDialog(1),
        MakeDialog(30),
    });

    ASSERT_EQ(data_source.GetDataCount(), 5);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(30)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(14)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(10)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(9)));
    ASSERT_EQ(data_source.GetDialogAtIndex(4)->ID(), DialogID(DialogTransientID(1)));
}



TEST(DialogDataSourceTest, AddDialogs_AddToFront) {

    DialogDataSource data_source;

    ListDataSourceDataAddInfo add_info;
    auto sub = data_source.DataAddEvent().Subscribe(
        [&add_info](const ListDataSourceDataAddInfo& event_info) {
            add_info = event_info;
        });

    data_source.AddDialogs({
        MakeDialog(50),
        MakeDialog(40),
        MakeDialog(30),
    });

    data_source.AddDialogs({ MakeDialog(60) });
    ASSERT_EQ(data_source.GetDataCount(), 4);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(60)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(30)));
    ASSERT_EQ(add_info.index, 0);
    ASSERT_EQ(add_info.count, 1);

    data_source.AddDialogs({
        MakeDialog(61),
        MakeDialog(62),
        MakeDialog(63),
    });
    ASSERT_EQ(data_source.GetDataCount(), 7);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(63)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(62)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(61)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(60)));
    ASSERT_EQ(data_source.GetDialogAtIndex(4)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(5)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(data_source.GetDialogAtIndex(6)->ID(), DialogID(DialogTransientID(30)));
    ASSERT_EQ(add_info.index, 0);
    ASSERT_EQ(add_info.count, 3);
}


TEST(DialogDataSourceTest, AddDialogs_AddToBack) {

    DialogDataSource data_source;

    ListDataSourceDataAddInfo add_info;
    auto sub = data_source.DataAddEvent().Subscribe(
        [&add_info](const ListDataSourceDataAddInfo& event_info) {
            add_info = event_info;
        });

    data_source.AddDialogs({
        MakeDialog(50),
        MakeDialog(40),
        MakeDialog(30),
    });

    data_source.AddDialogs({ MakeDialog(20) });
    ASSERT_EQ(data_source.GetDataCount(), 4);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(30)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(20)));
    ASSERT_EQ(add_info.index, 3);
    ASSERT_EQ(add_info.count, 1);

    data_source.AddDialogs({
        MakeDialog(14),
        MakeDialog(15),
    });
    ASSERT_EQ(data_source.GetDataCount(), 6);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(30)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(20)));
    ASSERT_EQ(data_source.GetDialogAtIndex(4)->ID(), DialogID(DialogTransientID(15)));
    ASSERT_EQ(data_source.GetDialogAtIndex(5)->ID(), DialogID(DialogTransientID(14)));
    ASSERT_EQ(add_info.index, 4);
    ASSERT_EQ(add_info.count, 2);
}


TEST(DialogDataSourceTest, AddDialogs_AddToMiddle) {

    DialogDataSource data_source;

    ListDataSourceDataAddInfo add_info;
    auto sub = data_source.DataAddEvent().Subscribe(
        [&add_info](const ListDataSourceDataAddInfo& event_info) {
            add_info = event_info;
        });

    data_source.AddDialogs({
        MakeDialog(50),
        MakeDialog(40),
    });

    data_source.AddDialogs({ MakeDialog(45) });
    ASSERT_EQ(data_source.GetDataCount(), 3);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(45)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(add_info.index, 1);
    ASSERT_EQ(add_info.count, 1);

    data_source.AddDialogs({ 
        MakeDialog(41),
        MakeDialog(42),
        MakeDialog(43),
        MakeDialog(44),
    });
    ASSERT_EQ(data_source.GetDataCount(), 7);
    ASSERT_EQ(data_source.GetDialogAtIndex(0)->ID(), DialogID(DialogTransientID(50)));
    ASSERT_EQ(data_source.GetDialogAtIndex(1)->ID(), DialogID(DialogTransientID(45)));
    ASSERT_EQ(data_source.GetDialogAtIndex(2)->ID(), DialogID(DialogTransientID(44)));
    ASSERT_EQ(data_source.GetDialogAtIndex(3)->ID(), DialogID(DialogTransientID(43)));
    ASSERT_EQ(data_source.GetDialogAtIndex(4)->ID(), DialogID(DialogTransientID(42)));
    ASSERT_EQ(data_source.GetDialogAtIndex(5)->ID(), DialogID(DialogTransientID(41)));
    ASSERT_EQ(data_source.GetDialogAtIndex(6)->ID(), DialogID(DialogTransientID(40)));
    ASSERT_EQ(add_info.index, 2);
    ASSERT_EQ(add_info.count, 4);
}


TEST(DialogDataSourceTest, AddDialogs_AddSingleEqualDialog) {

    ListDataSourceDataUpdateInfo update_event_info;

    DialogDataSource data_source;
    auto sub = data_source.DataUpdateEvent().Subscribe(
        [&update_event_info](const ListDataSourceDataUpdateInfo& event_info) {
            update_event_info = event_info;
        });

    data_source.AddDialogs({
        MakeDialog(14),
        MakeDialog(10),
        MakeDialog(9),
    });

    data_source.AddDialogs({ MakeDialog(14) });
    ASSERT_EQ(data_source.GetDataCount(), 3);
    ASSERT_EQ(update_event_info.index, 0);
    ASSERT_EQ(update_event_info.count, 1);

    data_source.AddDialogs({ MakeDialog(10) });
    ASSERT_EQ(data_source.GetDataCount(), 3);
    ASSERT_EQ(update_event_info.index, 1);
    ASSERT_EQ(update_event_info.count, 1);

    data_source.AddDialogs({ MakeDialog(9) });
    ASSERT_EQ(data_source.GetDataCount(), 3);
    ASSERT_EQ(update_event_info.index, 2);
    ASSERT_EQ(update_event_info.count, 1);
}


TEST(DialogDataSourceTest, AddDialogs_AddContinousEqualDialog) {

    std::vector<ListDataSourceDataUpdateInfo> update_event_infos;

    DialogDataSource data_source;
    auto sub = data_source.DataUpdateEvent().Subscribe(
        [&update_event_infos](const ListDataSourceDataUpdateInfo& event_info) {
            update_event_infos.push_back(event_info);
        });

    data_source.AddDialogs({
        MakeDialog(14),
        MakeDialog(10),
        MakeDialog(9),
    });

    data_source.AddDialogs({
        MakeDialog(14),
        MakeDialog(10),
        MakeDialog(9),
    });
    ASSERT_EQ(data_source.GetDataCount(), 3);
    ASSERT_EQ(update_event_infos.size(), 3);
    ASSERT_EQ(update_event_infos[0].index, 0);
    ASSERT_EQ(update_event_infos[0].count, 1);
    ASSERT_EQ(update_event_infos[1].index, 1);
    ASSERT_EQ(update_event_infos[1].count, 1);
    ASSERT_EQ(update_event_infos[2].index, 2);
    ASSERT_EQ(update_event_infos[2].count, 1);
}