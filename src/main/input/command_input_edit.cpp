#include "main/input/command_input_edit.h"
#include <tom.h>
#include <zaf/base/auto_reset.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/com_object.h>
#include <zaf/clipboard/clipboard.h>
#include <zaf/object/type_definition.h>
#include "main/input/clipboard_data.h"

namespace ra::main::input {

ZAF_DEFINE_TYPE(CommandInputEdit)
ZAF_DEFINE_TYPE_END;

void CommandInputEdit::Initialize() {

    __super::Initialize();

    this->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    this->SetAcceptReturn(false);
    this->SetMaxLength(10000);
    this->SetOLECallback(zaf::As<zaf::rich_edit::OLECallback>(shared_from_this()));

    Subscriptions() += this->FocusLostEvent().Subscribe(std::bind([this]() {

        auto selection_range = this->GetSelectionRange();
        this->SetSelectionRange(zaf::Range{ selection_range.EndIndex(), 0});
    }));
}


void CommandInputEdit::SetStyle(CommandDisplayStyle style) {
    style_ = style;
}


utility::CommandLine CommandInputEdit::GetInputCommandLine() {

    auto ole_interface = this->GetOLEInterface();
    return utility::CommandLine(this->Text(), [ole_interface](int object_index) {

        auto type = utility::CommandLinePieceType::TextBlock;
        std::wstring content;

        try {
            auto object = ole_interface.GetEmbeddedObjectAt(object_index);
            if (auto text_block_object = zaf::As<TextBlockObject>(object)) {
                type = utility::CommandLinePieceType::TextBlock;
                content = text_block_object->Text();
            }
            else if (auto active_path_object = zaf::As<ActivePathObject>(object)) {
                type = utility::CommandLinePieceType::ActivePath;
                content = active_path_object->Text();
            }
        }
        catch (const zaf::Error&) {

        }

        return utility::CommandLinePiece{ type, std::move(content) };
    });
}


CommandInputContent CommandInputEdit::GetInputContent() {
    return CommandInputContent::FromRichEdit(*this);
}


void CommandInputEdit::SetInputContent(const CommandInputContent& content) {

    this->SetText({});

    content.Visit(
        [this](const std::variant<std::wstring_view, zaf::rich_edit::ObjectInfo>& variant) {
    
        auto string = std::get_if<std::wstring_view>(&variant);
        if (string) {
            this->InsertText(std::wstring{ *string });
        }

        auto object_info = std::get_if<zaf::rich_edit::ObjectInfo>(&variant);
        if (object_info) {

            auto argument_object = zaf::As<ArgumentObject>(
                zaf::rich_edit::EmbeddedObject::TryFromCOMPtr(object_info->Object()));

            if (argument_object) {
                InsertArgumentObject(argument_object);
            }
        }
    });
}


zaf::Observable<zaf::None> CommandInputEdit::CommandChangedEvent() {
    return command_changed_event_.AsObservable();
}


void CommandInputEdit::OnTextChanged(const zaf::rich_edit::TextChangedInfo& event_info) {

    __super::OnTextChanged(event_info);

    RaiseCommandChangedEvent();
}


void CommandInputEdit::RaiseCommandChangedEvent() {
    command_changed_event_.AsObserver().OnNext({});
}


void CommandInputEdit::OnKeyDown(const zaf::KeyDownInfo& event_info) {

    if (event_info.Message().Key() == zaf::Key::V) {
        if (GetKeyState(VK_CONTROL) >> 15) {
            HandlePaste(event_info);
            event_info.MarkAsHandled();
        }
    }
    else if (event_info.Message().Key() == static_cast<zaf::Key>(L'2')) {
        if (GetKeyState(VK_CONTROL) >> 15) {
            InsertActivePathFromClipboard();
            event_info.MarkAsHandled();
        }
    }

    __super::OnKeyDown(event_info);
}


void CommandInputEdit::HandlePaste(const zaf::KeyDownInfo& event_info) {

    auto auto_reset = zaf::MakeAutoReset(suppress_text_block_);

    if (GetKeyState(VK_SHIFT) >> 15) {
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



void CommandInputEdit::InsertArgumentObject(
    const std::shared_ptr<ArgumentObject>& object) {

    Subscriptions() += object->TextChangedEvent().Subscribe(
        std::bind(&CommandInputEdit::RaiseCommandChangedEvent, this));

    object->SetStyle(style_);

    this->InsertObject(object);
}


std::shared_ptr<TextBlockObject> CommandInputEdit::InsertTextBlockObjectWithText(
    const std::wstring& text) {

    auto text_block_object = zaf::Create<TextBlockObject>(text);
    InsertArgumentObject(text_block_object);
    return text_block_object;
}



void CommandInputEdit::InsertTextBlockObjectByKey() {

    auto object = InsertTextBlockObjectWithText({});

    auto selection_range = this->GetSelectionRange();
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


zaf::rich_edit::OperationResult CommandInputEdit::CanInsertClipboardData(
    zaf::rich_edit::ClipboardOperation operation,
    const zaf::clipboard::DataObject& data_object,
    zaf::clipboard::FormatType format_type) {

    if (format_type == ClipboardData::PrivateFormatType) {
        return zaf::rich_edit::OperationResult::OK;
    }

    return zaf::rich_edit::OperationResult::Pending;
}


zaf::rich_edit::OperationResult CommandInputEdit::InsertClipboardData(
    zaf::rich_edit::ClipboardOperation operation,
    const zaf::clipboard::DataObject& data_object,
    zaf::clipboard::FormatType& format_type) {

    try {
        bool has_private_format{};
        bool has_text_format{};

        auto descriptor_enumerator = data_object.EnumerateDataDescriptors();
        while (auto descriptor = descriptor_enumerator.Next()) {
            if (descriptor->FormatType() == ClipboardData::PrivateFormatType) {
                has_private_format = true;
            }
            else if (descriptor->FormatType() == zaf::clipboard::FormatType::Text) {
                has_text_format = true;
            }
        }

        if (has_private_format) {
            InsertPrivateClipboardData(data_object);
            return zaf::rich_edit::OperationResult::OK;
        }
        else if (has_text_format) {
            InsertTextData(data_object);
            return zaf::rich_edit::OperationResult::OK;
        }
        return zaf::rich_edit::OperationResult::Cancel;
    }
    catch (const zaf::Error&) {
        return zaf::rich_edit::OperationResult::Cancel;
    }
}


void CommandInputEdit::InsertPrivateClipboardData(const zaf::clipboard::DataObject& data_object) {

    auto descriptor = 
        zaf::clipboard::DataDescriptor::FromFormatType(ClipboardData::PrivateFormatType);

    ClipboardData clipboard_data;
    data_object.GetData(descriptor, clipboard_data);

    for (const auto& each_object : clipboard_data.Objects()) {

        if (auto string_data = zaf::As<zaf::WideString>(each_object)) {
            this->InsertText(string_data->Value());
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
        this->InsertText(text);
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


zaf::rich_edit::OperationResult CommandInputEdit::GetClipboardData(
    zaf::rich_edit::ClipboardOperation operation,
    const zaf::Range& text_range,
    zaf::clipboard::DataObject& data_object) {

    auto text_in_range = this->GetTextInRange(text_range);

    std::vector<std::size_t> object_indexes;
    for (auto index : zaf::Range(0, text_in_range.length())) {
        if (text_in_range[index] == WCH_EMBEDDING) {
            object_indexes.push_back(index);
        }
    }

    if (object_indexes.empty()) {
        return zaf::rich_edit::OperationResult::Pending;
    }

    ClipboardData clipboard_data;

    std::size_t string_begin_index{};
    for (auto each_object_index : object_indexes) {

        auto string = text_in_range.substr(
            string_begin_index, 
            each_object_index - string_begin_index);

        if (!string.empty()) {
            clipboard_data.AddObject(zaf::Box(string));
        }

        string_begin_index = each_object_index + 1;

        auto argument_data = GetArgumentDataAtIndex(each_object_index + text_range.index);
        if (argument_data) {
            clipboard_data.AddObject(argument_data);
        }
    }

    if (string_begin_index < text_in_range.length()) {

        auto string = text_in_range.substr(string_begin_index);
        if (!string.empty()) {
            clipboard_data.AddObject(zaf::Box(string));
        }
    }

    data_object.SetData(
        zaf::clipboard::DataDescriptor::FromFormatType(zaf::clipboard::FormatType::Text), 
        clipboard_data);

    data_object.SetData(
        zaf::clipboard::DataDescriptor::FromFormatType(ClipboardData::PrivateFormatType),
        clipboard_data);

    return zaf::rich_edit::OperationResult::OK;
}


std::shared_ptr<zaf::Object> CommandInputEdit::GetArgumentDataAtIndex(std::size_t index) {

    auto text_document = this->GetOLEInterface().Ptr().Query<ITextDocument>();
    if (!text_document) {
        return nullptr;
    }
     
    zaf::COMPtr<ITextRange> text_range;
    HRESULT hresult = text_document->Range(
        static_cast<long>(index),
        static_cast<long>(index),
        text_range.Reset());

    if (FAILED(hresult)) {
        return nullptr;
    }

    zaf::COMPtr<IUnknown> unknown;
    hresult = text_range->GetEmbeddedObject(unknown.Reset());
    if (FAILED(hresult)) {
        return nullptr;
    }

    auto embedded_object = zaf::rich_edit::EmbeddedObject::TryFromCOMPtr(unknown);
    if (!embedded_object) {
        return nullptr;
    }

    auto argument_object = zaf::As<ArgumentObject>(embedded_object);
    if (!argument_object) {
        return nullptr;
    }

    return argument_object->Data();
}

}