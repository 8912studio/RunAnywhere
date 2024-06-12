#include <gtest/gtest.h>
#include <zaf/object/boxing/boxing.h>
#include "main/input/active_path_data.h"
#include "main/input/clipboard_data.h"
#include "main/input/text_block_data.h"

using namespace ra::main::input;

namespace {

std::wstring GetTextFromMedium(const zaf::clipboard::Medium& medium) {

    std::wstring result;
    medium.VisitGlobalMem([&result](const zaf::GlobalMem& mem) {
        auto lock = mem.Lock();
        result.assign(reinterpret_cast<const wchar_t*>(lock.Pointer()));
    });
    return result;
}

}

TEST(ClipboardDataTest, WriteToMediumAsText) {

    auto descriptor = 
        zaf::clipboard::DataDescriptor::FromFormatType(zaf::clipboard::FormatType::Text);

    //Empty data.
    {
        ClipboardData data;
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"");
    }

    //String data only.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"StringData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"StringData");
    }

    //Text block data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<TextBlockData>(L"TextBlockData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"TextBlockData");
    }

    //Active path data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<ActivePathData>(L"ActivePathData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"@=ActivePathData");
    }

    //String data and argument data combination.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"String1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block2"));
        data.AddObject(zaf::Box(L"String2"));
        data.AddObject(zaf::Box(L"String3"));
        data.AddObject(std::make_shared<ActivePathData>(L"ActivePath"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block3"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"String1 Block1 Block2 String2 String3 @=ActivePath Block3");
    }
}


TEST(ClipboardDataTest, WriteToMediumAsPrivateFormat) {

    auto descriptor =
        zaf::clipboard::DataDescriptor::FromFormatType(ClipboardData::PrivateFormatType);

    //Empty data.
    {
        ClipboardData data;
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[]})");
    }

    //String data only.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"StringData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[{"type":0,"value":"U3RyaW5nRGF0YQ=="}]})");
    }

    //Text block data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<TextBlockData>(L"TextBlockData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[{"type":1,"value":"VGV4dEJsb2NrRGF0YQ=="}]})");
    }

    //Active path data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<ActivePathData>(L"ActivePathData"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[{"type":2,"value":"QWN0aXZlUGF0aERhdGE="}]})");
    }

    //String data and argument data combination.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"String1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block2"));
        data.AddObject(zaf::Box(L"String2"));
        data.AddObject(zaf::Box(L"String3"));
        data.AddObject(std::make_shared<ActivePathData>(L"ActivePath"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block3"));
        auto medium = data.WriteToMedium(descriptor);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, 
            LR"({"items":[)"
            LR"({"type":0,"value":"U3RyaW5nMQ=="},)"
            LR"({"type":1,"value":"QmxvY2sx"},)"
            LR"({"type":1,"value":"QmxvY2sy"},)"
            LR"({"type":0,"value":"U3RyaW5nMg=="},)"
            LR"({"type":0,"value":"U3RyaW5nMw=="},)"
            LR"({"type":2,"value":"QWN0aXZlUGF0aA=="},)"
            LR"({"type":1,"value":"QmxvY2sz"})"
            LR"(]})"
        );
    }
}