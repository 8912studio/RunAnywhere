#include "about_window.h"
#include <zaf/reflection/reflection_type_definition.h>

namespace ra {

ZAF_DEFINE_REFLECTION_TYPE(AboutWindow)
ZAF_DEFINE_RESOURCE_URI(L"res:///about_window.xaml")
ZAF_DEFINE_END

std::weak_ptr<AboutWindow> g_instance;

void AboutWindow::ShowInstance() {

    auto instance = g_instance.lock();
    if (!instance) {
        instance = zaf::Create<AboutWindow>();
        g_instance = instance;
    }

    instance->Show();
    SetForegroundWindow(instance->GetHandle());
}

}