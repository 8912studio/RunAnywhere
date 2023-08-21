#include <gtest/gtest.h>
#include <zaf/object/boxing/boxing.h>
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

TEST(ClipboardDataTest, SaveToMediumAsText) {

    zaf::clipboard::Format format{ zaf::clipboard::FormatType::Text };

    //Empty data.
    {
        ClipboardData data;
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"");
    }

    //String data only.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"StringData"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"StringData");
    }

    //Text block data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<TextBlockData>(L"TextBlockData"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"TextBlockData");
    }

    //String data and text block data combinnation.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"String1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block2"));
        data.AddObject(zaf::Box(L"String2"));
        data.AddObject(zaf::Box(L"String3"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block3"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, L"String1 Block1 Block2 String2 String3 Block3");
    }
}


TEST(ClipboardDataTest, SaveToMediumAsPrivateFormat) {

    zaf::clipboard::Format format{ ClipboardData::PrivateFormatType };

    //Empty data.
    {
        ClipboardData data;
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[]})");
    }

    //String data only.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"StringData"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[{"type":0,"value":"U3RyaW5nRGF0YQ=="}]})");
    }

    //Text block data only.
    {
        ClipboardData data;
        data.AddObject(std::make_shared<TextBlockData>(L"TextBlockData"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, LR"({"items":[{"type":1,"value":"VGV4dEJsb2NrRGF0YQ=="}]})");
    }

    //String data and text block data combinnation.
    {
        ClipboardData data;
        data.AddObject(zaf::Box(L"String1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block1"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block2"));
        data.AddObject(zaf::Box(L"String2"));
        data.AddObject(zaf::Box(L"String3"));
        data.AddObject(std::make_shared<TextBlockData>(L"Block3"));
        auto medium = data.SaveToMedium(format);
        auto text = GetTextFromMedium(medium);
        ASSERT_EQ(text, 
            LR"({"items":[)"
            LR"({"type":0,"value":"U3RyaW5nMQ=="},)"
            LR"({"type":1,"value":"QmxvY2sx"},)"
            LR"({"type":1,"value":"QmxvY2sy"},)"
            LR"({"type":0,"value":"U3RyaW5nMg=="},)"
            LR"({"type":0,"value":"U3RyaW5nMw=="},)"
            LR"({"type":1,"value":"QmxvY2sz"})"
            LR"(]})"
        );
    }
}