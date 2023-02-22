#pragma once

#include <zaf/window/message/activate_message.h>
#include <zaf/window/window.h>

namespace ra::utility {

class ThinBorderWindow : public zaf::Window {
protected:
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;
    void OnMessageHandled(const zaf::MessageHandledInfo& event_info) override;

private:
    void HandleCalculateNonClientSizeMessage(const zaf::Message& message);
    void HandleActivateMessage(const zaf::ActivateMessage& message);
};

}