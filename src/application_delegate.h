#pragma once

#include <zaf/application_delegate.h>
#include <zaf/window/message_only_window.h>

namespace ra {

class ApplicationDelegate : public zaf::ApplicationDelegate {
public:
	void ApplicationBeginRun(const zaf::ApplicationBeginRunInfo&) override;
	void ApplicationEndRun(const zaf::ApplicationEndRunInfo&) override;
	void SessionEnded(const zaf::SessionEndedInfo&) override;

private:
	void ShowTryIcon();
	void PopupMenu();

private:
	std::unique_ptr<zaf::MessageOnlyWindow> tray_icon_message_window_;
	HMENU menu_{};
};

}