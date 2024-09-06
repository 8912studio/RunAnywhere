#pragma once

#include <zaf/base/none.h>
#include <zaf/control/text_box.h>
#include <zaf/rx/subject.h>
#include "main/command_display_style.h"
#include "main/input/active_path_object.h"
#include "main/input/command_input_content.h"
#include "main/input/text_block_object.h"
#include "utility/command_line.h"

namespace ra::main::input {

class CommandInputEdit : public zaf::TextBox {
public:
    ZAF_OBJECT;

    void SetStyle(CommandDisplayStyle style);

    utility::CommandLine GetInputCommandLine();

    CommandInputContent GetInputContent();
    void SetInputContent(const CommandInputContent& content);

    //Use this event to observe input command changed event, rather than TextChangedEvent.
    zaf::Observable<zaf::None> CommandChangedEvent();

protected:
    void Initialize() override;
    void OnTextChanged(const zaf::TextChangedInfo& event_info) override;
    void OnKeyDown(const zaf::KeyDownInfo& event_info) override;
    void OnSysKeyDown(const zaf::SysKeyDownInfo& event_info) override;
    void OnCopying(const zaf::textual::CopyingInfo& event_info) override;
    void OnPasting(const zaf::textual::PastingInfo& event_info) override;

private:
    void HandlePaste(const zaf::KeyDownInfo& event_info);

    void RaiseCommandChangedEvent();
    void InsertArgumentObject(const std::shared_ptr<ArgumentObject>& object);

    std::shared_ptr<TextBlockObject> InsertTextBlockObjectWithText(const std::wstring& text);
    std::shared_ptr<ActivePathObject> InsertActivePathObjectWithText(std::wstring text);
    
    void InsertActivePathFromClipboard();
    void InsertTextBlockObjectByKey();

    void InsertPrivateClipboardData(const zaf::clipboard::DataObject& data_object);
    void InsertTextData(const zaf::clipboard::DataObject& data_object);
    void InsertTextOrTextBlockObject(const std::wstring& text);
    bool ShouldInsertTextBlockObject(const std::wstring& text) const;

private:
    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
    zaf::Subject<zaf::None> command_changed_event_;
    bool suppress_text_block_{};
};

ZAF_OBJECT_BEGIN(CommandInputEdit)
ZAF_OBJECT_END;

}