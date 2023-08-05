#include "main/command_input_edit.h"
#include <tom.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/com_object.h>
#include <zaf/clipboard/clipboard.h>
#include <zaf/object/type_definition.h>
#include "main/clipboard_data.h"

namespace ra {

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

        try {

            auto object = ole_interface.GetEmbeddedObjectAt(object_index);
            auto text_block_object = zaf::As<TextBlockObject>(object);
            if (text_block_object) {
                return text_block_object->Text();
            }
            return std::wstring{};
        }
        catch (const zaf::Error&) {
            return std::wstring{};
        }
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

            auto text_block_object = zaf::As<TextBlockObject>(
                zaf::rich_edit::EmbeddedObject::TryFromCOMPtr(object_info->Object()));

            if (text_block_object) {
                InsertTextBlockObject(text_block_object);
            }
        }
    });
}


zaf::Observable<zaf::None> CommandInputEdit::CommandChangedEvent() {
    return command_changed_event_.GetObservable();
}


bool CommandInputEdit::ShouldInsertTextBlockObject(const std::wstring& text) {

    if (text.length() > 30) {
        return true;
    }

    if (zaf::Contain(text, L'\r') || zaf::Contain(text, L'\n')) {
        return true;
    }

    return false;
}


std::shared_ptr<TextBlockObject> CommandInputEdit::InsertTextBlockObjectWithText(
    const std::wstring& text) {

    auto text_block_object = zaf::Create<TextBlockObject>(text);
    InsertTextBlockObject(text_block_object);
    return text_block_object;
}


void CommandInputEdit::InsertTextBlockObject(
    const std::shared_ptr<TextBlockObject>& object) {

    Subscriptions() += object->TextChangedEvent().Subscribe(
        std::bind(&CommandInputEdit::RaiseCommandChangedEvent, this));

    object->SetStyle(style_);

    this->InsertObject(object);
}


void CommandInputEdit::OnTextChanged(const zaf::TextChangedInfo& event_info) {
    RaiseCommandChangedEvent();
}


void CommandInputEdit::RaiseCommandChangedEvent() {
    command_changed_event_.GetObserver().OnNext({});
}


void CommandInputEdit::OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) {

    if (event_info.Message().VirtualKey() == L'T') {
        InsertTextBlockObjectByKey();
        event_info.MarkAsHandled();
    }

    __super::OnKeyDown(event_info);
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

        auto format_enumerator = data_object.EnumerateFormats();
        while (auto format = format_enumerator.Next()) {
            if (format->Type() == ClipboardData::PrivateFormatType) {
                has_private_format = true;
            }
            else if (format->Type() == zaf::clipboard::FormatType::Text) {
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

    zaf::clipboard::Format format{ ClipboardData::PrivateFormatType };

    auto clipboard_data = zaf::As<ClipboardData>(data_object.GetData(format.Type()));
    if (!clipboard_data) {
        return;
    }

    for (const auto& each_object : clipboard_data->Objects()) {

        if (auto string_data = zaf::As<zaf::WideString>(each_object)) {
            this->InsertText(string_data->Value());
        }
        else if (auto text_block_data = zaf::As<TextBlockData>(each_object)) {
            auto text_block_object = zaf::Create<TextBlockObject>(text_block_data);
            InsertTextBlockObject(text_block_object);
        }
    }
}


void CommandInputEdit::InsertTextData(const zaf::clipboard::DataObject& data_object) {

    auto text = data_object.GetText();

    if (ShouldInsertTextBlockObject(text)) {
        InsertTextBlockObjectWithText(text);
    }
    else {
        this->InsertText(text);
    }
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

    auto clipboard_data = std::make_shared<ClipboardData>();

    std::size_t string_begin_index{};
    for (auto each_object_index : object_indexes) {

        auto string = text_in_range.substr(
            string_begin_index, 
            each_object_index - string_begin_index);

        if (!string.empty()) {
            clipboard_data->AddObject(zaf::Box(string));
        }

        string_begin_index = each_object_index + 1;

        auto text_block_data = GetTextBlockDataAtIndex(each_object_index + text_range.index);
        if (text_block_data) {
            clipboard_data->AddObject(text_block_data);
        }
    }

    if (string_begin_index < text_in_range.length()) {

        auto string = text_in_range.substr(string_begin_index);
        if (!string.empty()) {
            clipboard_data->AddObject(zaf::Box(string));
        }
    }

    data_object.SetData(zaf::clipboard::FormatType::Text, clipboard_data);
    data_object.SetData(ClipboardData::PrivateFormatType, clipboard_data);
    return zaf::rich_edit::OperationResult::OK;
}


std::shared_ptr<zaf::Object> CommandInputEdit::GetTextBlockDataAtIndex(std::size_t index) {

    auto text_document = this->GetOLEInterface().Inner().Query<ITextDocument>();
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

    auto text_block_object = zaf::As<TextBlockObject>(embedded_object);
    if (!text_block_object) {
        return nullptr;
    }

    return zaf::Create<TextBlockData>(text_block_object->Text());
}

}