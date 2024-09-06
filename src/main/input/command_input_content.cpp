#include "main/input/command_input_content.h"

namespace ra::main::input {
namespace {

constexpr wchar_t ObjectReplacementChar = L'\ufffc';

}

CommandInputContent CommandInputContent::FromRichEdit(const zaf::RichEdit& rich_edit) {

    auto text = rich_edit.Text();
    std::vector<zaf::rich_edit::ObjectInfo> objects;

    auto ole_interface = rich_edit.GetOLEInterface();
    for (std::size_t index = 0; index < text.length(); ++index) {

        if (text[index] == ObjectReplacementChar) {
            objects.push_back(ole_interface.GetObjectInfoAt(objects.size()));
        }
    }

    return CommandInputContent{ std::move(text), std::move(objects) };
}


CommandInputContent CommandInputContent::FromTextBox(const zaf::TextBox& text_box) {
    
    const auto& text = text_box.Text();
    std::vector<ObjectInfo> objects;

    for (auto index : zaf::Range{ 0, text.length() }) {

        if (text[index] != zaf::textual::ObjectReplacementChar) {
            continue;
        }

        auto object = text_box.GetInlineObjectAtIndex(index);
        if (!object) {
            continue;
        }

        objects.emplace_back(index, object->Clone());
    }

    return CommandInputContent{ text, std::move(objects) };
}


CommandInputContent::CommandInputContent(
    std::wstring text,
    std::vector<zaf::rich_edit::ObjectInfo> objects) 
    :
    text_(std::move(text)),
    objects_(std::move(objects)) {

}


CommandInputContent::CommandInputContent(std::wstring text, std::vector<ObjectInfo> objects) :
    text_(std::move(text)),
    inline_objects_(std::move(objects)) {

}


void CommandInputContent::Visit(const Visitor& visitor) const {

    auto text_iterator = text_.begin();

    for (const auto& each_object : inline_objects_) {

        auto text_end_iterator = text_.begin() + each_object.index;
        visitor(std::wstring_view{ text_iterator, text_end_iterator  });
        text_iterator = text_end_iterator + 1;

        visitor(each_object);
    }

    if (text_iterator != text_.end()) {
        visitor(std::wstring_view{ text_iterator, text_.end() });
    }
}

}