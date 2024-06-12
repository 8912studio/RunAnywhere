#include "license_window.h"
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include <zaf/resource/resource_factory.h>

namespace ra {
namespace {

std::weak_ptr<LicenseWindow> g_instance;

std::wstring GetLicenseContent() {

    try {

        auto license_stream = zaf::ResourceFactory::Instance().LoadURI(L"res:///LICENSE");

        return zaf::FromUTF8String(std::string_view{
            reinterpret_cast<const char*>(license_stream.UnderlyingBuffer()),
            license_stream.Size(),
        });
    }
    catch (const zaf::Error&) {
        return {};
    }
}

}


ZAF_DEFINE_TYPE(LicenseWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///license_window.xaml")
ZAF_DEFINE_TYPE_END


void LicenseWindow::ShowInstance() {

    auto instance = g_instance.lock();
    if (!instance) {
        instance = zaf::Init(new LicenseWindow{});
        g_instance = instance;
    }

    instance->Show();
    SetForegroundWindow(instance->Handle());
    BringWindowToTop(instance->Handle());
}


void LicenseWindow::AfterParse() {

    __super::AfterParse();

    InitializeScrollableControl();
    InitializeTextBox();
}


void LicenseWindow::InitializeScrollableControl() {

    auto scroll_bar = scrollableControl->VerticalScrollBar();
    scroll_bar->SetSmallChange(20);
    scroll_bar->SetLargeChange(200);
}


void LicenseWindow::InitializeTextBox() {

    licenseContentTextBox->SetText(GetLicenseContent());
}

}