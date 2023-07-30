#pragma once

#include <zaf/base/none.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/rich_edit/ole_callback.h>
#include <zaf/rx/subject.h>
#include "main/command_input_content.h"
#include "main/text_block_object.h"
#include "utility/command_line.h"

namespace ra {

class CommandInputEdit : public zaf::RichEdit, public zaf::rich_edit::OLECallback {
public:
    ZAF_DECLARE_TYPE;

    utility::CommandLine GetInputCommandLine();

    CommandInputContent GetInputContent();
    void SetInputContent(const CommandInputContent& content);

    //Use this event to observe input command changed event, rather than TextChangedEvent.
    zaf::Observable<zaf::None> CommandChangedEvent();

protected:
    void Initialize() override;
    void OnTextChanged(const zaf::TextChangedInfo& event_info) override;
    void OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) override;

private:
    static bool ShouldInsertTextBlockObject(const std::wstring& text);

private:
    std::shared_ptr<TextBlockObject> InsertTextBlockObject(const std::wstring& text);
    void RaiseCommandChangedEvent();
    void InsertTextBlockObjectByKey();

    zaf::rich_edit::OperationResult CanInsertClipboardData(
        zaf::rich_edit::ClipboardOperation operation,
        const zaf::clipboard::DataObject& data_object,
        zaf::clipboard::FormatType format_type) override;

    zaf::rich_edit::OperationResult InsertClipboardData(
        zaf::rich_edit::ClipboardOperation operation,
        const zaf::clipboard::DataObject& data_object,
        zaf::clipboard::FormatType& format_type) override;

    void InsertPrivateClipboardData(const zaf::clipboard::DataObject& data_object);
    void InsertTextData(const zaf::clipboard::DataObject& data_object);

    zaf::rich_edit::OperationResult GetClipboardData(
        zaf::rich_edit::ClipboardOperation operation,
        const zaf::Range& text_range,
        zaf::clipboard::DataObject& data_object) override;

    std::shared_ptr<zaf::Object> GetTextBlockDataAtIndex(std::size_t index);

private:
    zaf::Subject<zaf::None> command_changed_event_;
};

}