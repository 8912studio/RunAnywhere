#include "main/text_block_window.h"
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/creation.h>
#include <zaf/base/none.h>

namespace ra {

ZAF_DEFINE_TYPE(TextBlockWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/text_block_window.xaml")
ZAF_DEFINE_TYPE_END;

TextBlockWindow::TextBlockWindow(const zaf::COMObject<TextBlockObject>& object) : object_(object) {

    ZAF_EXPECT(object_);
}


void TextBlockWindow::AfterParse() {

    __super::AfterParse();

    textEdit->SetText(object_->Text());
}


void TextBlockWindow::OnDeactivated(const zaf::DeactivatedInfo& event_info) {

    __super::OnDeactivated(event_info);

    //Close window at next message loop to avoid focus issues.
    //TODO: Need more elegant method to schedule task at next message loop.
    Subscriptions() += zaf::rx::Create<zaf::None>([this](zaf::Observer<zaf::None>) {
        this->Close();
        return zaf::Subscription{};
    })
    .SubscribeOn(zaf::Scheduler::Main())
    .Subscribe();
}

}