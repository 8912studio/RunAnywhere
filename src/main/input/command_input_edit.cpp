#include "main/input/command_input_edit.h"
#include <zaf/base/auto_reset.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/com_object.h>
#include <zaf/clipboard/clipboard.h>
#include <zaf/control/textual/styled_text.h>
#include <zaf/input/keyboard.h>
#include "main/input/clipboard_data.h"

namespace ra::main::input {

ZAF_OBJECT_IMPL(CommandInputEdit);

void CommandInputEdit::Initialize() {

    __super::Initialize();

    this->SetIsEditable(true);
    this->SetParagraphAlignment(zaf::dwrite::ParagraphAlignment::Center);
    this->SetMaxLength(1000);

    Subscriptions() += this->FocusLostEvent().Subscribe(std::bind([this]() {
        auto selection_range = this->SelectionRange();
        this->SetSelectionRange(zaf::Range{ selection_range.EndIndex(), 0});
    }));
}


void CommandInputEdit::SetStyle(CommandDisplayStyle style) {
    style_ = style;
}


utility::CommandLine CommandInputEdit::GetInputCommandLine() {

    return utility::CommandLine(this->Text(), [this](int object_index) {

        auto type = utility::CommandLinePieceType::TextBlock;
        std::wstring content;

        auto iterator = this->StyledText().InlineObjects().begin() + object_index;
        auto object = iterator->Object();

        if (auto text_block_object = zaf::As<TextBlockObject>(object)) {
            type = utility::CommandLinePieceType::TextBlock;
            content = text_block_object->Text();
        }
        else if (auto active_path_object = zaf::As<ActivePathObject>(object)) {
            type = utility::CommandLinePieceType::ActivePath;
            content = active_path_object->Text();
        }

        return utility::CommandLinePiece{ type, std::move(content) };
    });
}


CommandInputContent CommandInputEdit::GetInputContent() {
    return CommandInputContent::FromTextBox(*this);
}


void CommandInputEdit::SetInputContent(const CommandInputContent& content) {

    this->SetText({});

    content.Visit(
        [this](const std::variant<std::wstring_view, CommandInputContent::ObjectInfo>& variant) {
    
        std::visit([this](const auto& data) {
        
            using DataType = std::decay_t<decltype(data)>;
            if constexpr (std::is_same_v<std::wstring_view, DataType>) {
                this->Input(std::wstring{ data });
            }
            else if constexpr (std::is_same_v<CommandInputContent::ObjectInfo, DataType>) {
                
                auto argument_object = zaf::As<ArgumentObject>(data.object->Clone());
                if (argument_object) {
                    InsertArgumentObject(argument_object);
                }
            }
        },
        variant);
    });
}


zaf::Observable<zaf::None> CommandInputEdit::CommandChangedEvent() {
    return command_changed_event_.AsObservable();
}


void CommandInputEdit::OnTextChanged(const zaf::TextChangedInfo& event_info) {

    __super::OnTextChanged(event_info);

    RaiseCommandChangedEvent();
}


void CommandInputEdit::RaiseCommandChangedEvent() {
    command_changed_event_.AsObserver().OnNext({});
}


void CommandInputEdit::OnKeyDown(const zaf::KeyDownInfo& event_info) {

    if (event_info.Message().Key() == zaf::Key::V) {
        if (zaf::Keyboard::IsCtrlDown()) {
            HandlePaste(event_info);
            event_info.MarkAsHandled();
        }
    }
    else if (event_info.Message().Key() == static_cast<zaf::Key>(L'2')) {
        if (zaf::Keyboard::IsCtrlDown()) {
            InsertActivePathFromClipboard();
            event_info.MarkAsHandled();
        }
    }

    __super::OnKeyDown(event_info);
}


void CommandInputEdit::HandlePaste(const zaf::KeyDownInfo& event_info) {

    auto auto_reset = zaf::MakeAutoReset(suppress_text_block_);

    if (zaf::Keyboard::IsShiftDown()) {
        suppress_text_block_ = true;
    }

    this->Paste();
}


void CommandInputEdit::OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) {

    if (event_info.Message().Key() == zaf::Key::T) {
        InsertTextBlockObjectByKey();
        event_info.MarkAsHandled();
    }

    __super::OnSysKeyDown(event_info);
}


void CommandInputEdit::OnCopying(const zaf::textual::CopyingInfo& event_info) {

    __super::OnCopying(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    std::wstring_view selected_text{
        this->Text().data() + event_info.SelectionRange().index,
        event_info.SelectionRange().length,
    };

    std::vector<std::size_t> object_indexes;
    for (auto index : zaf::Range(0, selected_text.length())) {
        if (selected_text[index] == zaf::textual::ObjectReplacementChar) {
            object_indexes.push_back(index);
        }
    }

    if (object_indexes.empty()) {
        return;
    }

    ClipboardData clipboard_data;

    std::size_t string_begin_index{};
    for (auto each_object_index : object_indexes) {

        auto string = selected_text.substr(
            string_begin_index,
            each_object_index - string_begin_index);

        if (!string.empty()) {
            clipboard_data.AddObject(zaf::Box(string));
        }

        string_begin_index = each_object_index + 1;

        auto argument_data = this->GetInlineObjectAtIndex(
            each_object_index + event_info.SelectionRange().index);

        if (argument_data) {
            clipboard_data.AddObject(argument_data);
        }
    }

    if (string_begin_index < selected_text.length()) {

        auto string = selected_text.substr(string_begin_index);
        if (!string.empty()) {
            clipboard_data.AddObject(zaf::Box(string));
        }
    }

    auto data_object = zaf::clipboard::DataObject::Create();

    data_object.SetData(
        zaf::clipboard::DataDescriptor::FromFormatType(zaf::clipboard::FormatType::Text),
        clipboard_data);

    data_object.SetData(
        zaf::clipboard::DataDescriptor::FromFormatType(ClipboardData::PrivateFormatType),
        clipboard_data);

    event_info.SetDataObject(std::move(data_object));
    event_info.MarkAsHandled();
}


void CommandInputEdit::OnPasting(const zaf::textual::PastingInfo& event_info) {

    __super::OnPasting(event_info);
    if (event_info.IsHandled()) {
        return;
    }

    try {
        bool has_private_format{};
        bool has_text_format{};

        auto descriptor_enumerator = event_info.DataObject().EnumerateDataDescriptors();
        while (auto descriptor = descriptor_enumerator.Next()) {
            if (descriptor->FormatType() == ClipboardData::PrivateFormatType) {
                has_private_format = true;
            }
            else if (descriptor->FormatType() == zaf::clipboard::FormatType::Text) {
                has_text_format = true;
            }
        }

        if (has_private_format) {
            InsertPrivateClipboardData(event_info.DataObject());
            event_info.MarkAsHandled();
        }
        else if (has_text_format) {
            InsertTextData(event_info.DataObject());
            event_info.MarkAsHandled();
        }
    }
    catch (...) {

    }
}


void CommandInputEdit::InsertArgumentObject(const std::shared_ptr<ArgumentObject>& object) {

    Subscriptions() += object->TextChangedEvent().Subscribe(
        std::bind(&CommandInputEdit::RaiseCommandChangedEvent, this));

    object->SetStyle(style_);

    this->Input(object);
}


std::shared_ptr<TextBlockObject> CommandInputEdit::InsertTextBlockObjectWithText(
    const std::wstring& text) {

    auto text_block_object = zaf::Create<TextBlockObject>(text);
    InsertArgumentObject(text_block_object);
    return text_block_object;
}


void CommandInputEdit::InsertTextBlockObjectByKey() {

    auto object = InsertTextBlockObjectWithText({});

    auto selection_range = this->SelectionRange();
    if (selection_range.index <= 0) {
        //Abnormal situation.
        return;
    }

    //Select the inserted object.
    selection_range.index--;
    selection_range.length = 1;
    this->SetSelectionRange(selection_range);

    object->OpenWindow();
}


std::shared_ptr<ActivePathObject> CommandInputEdit::InsertActivePathObjectWithText(
    std::wstring text) {

    auto object = zaf::Create<ActivePathObject>(std::move(text));
    InsertArgumentObject(object);
    return object;
}


void CommandInputEdit::InsertActivePathFromClipboard() {

    try {
        auto clipboard_text = zaf::clipboard::Clipboard::GetText();
        InsertActivePathObjectWithText(clipboard_text);
    }
    catch (const zaf::Error&) {

    }
}


void CommandInputEdit::InsertPrivateClipboardData(const zaf::clipboard::DataObject& data_object) {

    auto descriptor = 
        zaf::clipboard::DataDescriptor::FromFormatType(ClipboardData::PrivateFormatType);

    ClipboardData clipboard_data;
    data_object.GetData(descriptor, clipboard_data);

    for (const auto& each_object : clipboard_data.Objects()) {

        if (auto string_data = zaf::As<zaf::WideString>(each_object)) {
            this->Input(string_data->Value());
        }
        else if (auto text_block_data = zaf::As<TextBlockData>(each_object)) {
            auto text_block_object = zaf::Create<TextBlockObject>(text_block_data);
            InsertArgumentObject(text_block_object);
        }
        else if (auto active_path_data = zaf::As<ActivePathData>(each_object)) {
            auto active_path_object = zaf::Create<ActivePathObject>(active_path_data);
            InsertArgumentObject(active_path_object);
        }
    }
}


void CommandInputEdit::InsertTextData(const zaf::clipboard::DataObject& data_object) {
    InsertTextOrTextBlockObject(data_object.GetText());
}


void CommandInputEdit::InsertTextOrTextBlockObject(const std::wstring& text) {
    if (ShouldInsertTextBlockObject(text)) {
        InsertTextBlockObjectWithText(text);
    }
    else {
        this->Input(text);
    }
}


bool CommandInputEdit::ShouldInsertTextBlockObject(const std::wstring& text) const {

    if (suppress_text_block_) {
        return false;
    }

    if (text.length() > 30) {
        return true;
    }

    if (zaf::Contain(text, L'\r') || zaf::Contain(text, L'\n')) {
        return true;
    }

    return false;
}

}