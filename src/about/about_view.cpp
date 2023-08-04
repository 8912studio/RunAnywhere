#include "about/about_view.h"
#include <zaf/application.h>
#include <zaf/object/type_definition.h>
#include "license_window.h"

namespace ra {
namespace {

std::wstring GetVersionString() {

    auto exe_path = zaf::Application::Instance().GetExeFilePath().wstring();

    DWORD version_info_size = GetFileVersionInfoSize(exe_path.c_str(), nullptr);
    if (version_info_size == 0) {
        return {};
    }

    auto version_info_buffer = std::make_unique<std::byte[]>(version_info_size);
    BOOL is_succeeded = GetFileVersionInfo(
        exe_path.c_str(),
        0,
        version_info_size,
        version_info_buffer.get());

    if (!is_succeeded) {
        return {};
    }

    VS_FIXEDFILEINFO* file_info;
    is_succeeded = VerQueryValue(
        version_info_buffer.get(),
        L"\\",
        reinterpret_cast<LPVOID*>(&file_info),
        nullptr);

    if (!is_succeeded) {
        return {};
    }

    short major = HIWORD(file_info->dwProductVersionMS);
    short minor = LOWORD(file_info->dwProductVersionMS);
    short revision = HIWORD(file_info->dwProductVersionLS);
    short build = LOWORD(file_info->dwProductVersionLS);

    return
        std::to_wstring(major) + L'.' +
        std::to_wstring(minor) + L'.' +
        std::to_wstring(revision) + L'.' +
        std::to_wstring(build);
}

}

ZAF_DEFINE_TYPE(AboutView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///about/about_view.xaml")
ZAF_DEFINE_TYPE_END;

void AboutView::AfterParse() {

    __super::AfterParse();

    InitializeGithubButton();
    InitializeLicenseButton();

    versionLabel->SetText(GetVersionString());
}


void AboutView::InitializeGithubButton() {

    Subscriptions() += githubButton->ClickEvent().Subscribe(std::bind([this]() {

        auto window = Window();
        ShellExecute(
            window ? window->Handle() : nullptr,
            L"open",
            L"https://github.com/zplutor/runanywhere",
            nullptr,
            nullptr,
            SW_SHOW);
    }));
}


void AboutView::InitializeLicenseButton() {

    Subscriptions() += licenseButton->ClickEvent().Subscribe(std::bind([this]() {
        LicenseWindow::ShowInstance();
    }));
}

}