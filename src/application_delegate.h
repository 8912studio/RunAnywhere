#pragma once

#include <zaf/application_delegate.h>
#include <zaf/window/window.h>

namespace ra {

class ApplicationDelegate : public zaf::ApplicationDelegate {
public:
	void ApplicationBeginRun(const zaf::ApplicationBeginRunInfo&) override;
	void ApplicationEndRun(const zaf::ApplicationEndRunInfo&) override;
	void SessionEnded(const zaf::SessionEndedInfo&) override;

private:
	void InitializeTrayIconWindow();
	void ShowTryIcon();
	void PopupMenu();

private:
	UINT task_bar_create_message_id_{};
	std::shared_ptr<zaf::Window> tray_icon_window_;
	HMENU menu_{};
};

}