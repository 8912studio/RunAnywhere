#pragma once

#include <zaf/application_delegate.h>
#include <zaf/window/window.h>
#include "main_window.h"
#include "module_manager.h"

namespace ra {

class ApplicationDelegate : public zaf::ApplicationDelegate {
public:
	static std::shared_ptr<ApplicationDelegate> GetFromApplication();

public:
	void ReloadModules();

	void ApplicationBeginRun(const zaf::ApplicationBeginRunInfo&) override;
	void ApplicationEndRun(const zaf::ApplicationEndRunInfo&) override;
	void SessionEnded(const zaf::SessionEndedInfo&) override;

private:
	void InitializeTrayIconWindow();
	void HandleIPCMessage(const zaf::Message& message);
	void ShowTryIcon();
	void PopupMenu();
	void InitializeHotKey();

private:
	UINT task_bar_create_message_id_{};
	std::shared_ptr<zaf::Window> message_window_;
	HMENU menu_{};

	std::shared_ptr<ModuleManager> module_manager_;
	std::shared_ptr<MainWindow> main_window_;
};

}