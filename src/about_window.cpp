#include "about_window.h"
#include <shellapi.h>
#include <format>
#include <zaf/application.h>
#include "license_window.h"
#include "version.h"

namespace ra {

ZAF_OBJECT_IMPL(AboutWindow);

std::weak_ptr<AboutWindow> g_instance;

void AboutWindow::ShowInstance() {

    auto instance = g_instance.lock();
    if (!instance) {
        instance = zaf::Create<AboutWindow>();
        g_instance = instance;
    }

    instance->Show();
    SetForegroundWindow(instance->Handle());
    BringWindowToTop(instance->Handle());
}


void AboutWindow::AfterParse() {

    __super::AfterParse();

    InitializeVersionText();
    InitializeGithubButton();
    InitializeLicenseButton();
    InitializeCopyrightText();
}


void AboutWindow::InitializeVersionText() {
    versionLabel->SetText(std::format(L"{}.{}.{}.{}", MAJOR, MINOR, REVISION, BUILD));
}


void AboutWindow::InitializeGithubButton() {

    Subscriptions() += githubButton->ClickEvent().Subscribe(std::bind([this]() {
    
        ShellExecute(
            this->Handle(),
            L"open",
            L"https://github.com/zplutor/runanywhere", 
            nullptr, 
            nullptr, 
            SW_SHOW);
    }));
}


void AboutWindow::InitializeLicenseButton() {

    Subscriptions() += licenseButton->ClickEvent().Subscribe(std::bind([this]() {
        LicenseWindow::ShowInstance();
    }));
}


void AboutWindow::InitializeCopyrightText() {

    auto text = std::format(L"Copyright © 2022-{} 8912studio. All rights reserved.", YEAR);
    copyrightLabel->SetText(std::move(text));
}

}