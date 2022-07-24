#include "entry/main_entry.h"
#include <zaf/base/string/case_conversion.h>
#include "entry/application_switches.h"
#include "entry/general_entry.h"
#include "entry/register_entry.h"

namespace ra::entry {

EntryResult RunMainEntry(const std::wstring& command_line) {

    ApplicationSwithes switches{ command_line };
    if (switches.HasSwitch(RegisterSwitchName)) {
        return RunRegisterEntry();
    }
    else {
        return RunGeneralEntry(switches);
    }
}

}