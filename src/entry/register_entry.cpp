#include "entry/register_entry.h"
#include <Shlobj.h>
#include <zaf/application.h>
#include <zaf/base/registry/registry.h>

namespace ra::entry {
namespace {

constexpr const wchar_t* ProgID = L"RunAnywhere.Bundle.1";

void RegisterProgID() {

    auto prog_id_key = zaf::Registry::CurrentUser().CreateSubKey(
        L"Software\\Classes\\" + std::wstring(ProgID), 
        zaf::RegistryRights::Write);

    auto command_key = prog_id_key.CreateSubKey(
        L"Shell\\Import\\Command", 
        zaf::RegistryRights::Write);

    std::wstring command_line;
    command_line += L'\"';
    command_line += zaf::Application::Instance().GetExeFilePath().wstring();
    command_line += L"\" /import \"%1\"";

    command_key.SetStringValue({}, command_line);
}


void RegisterFileType() {

    auto file_type_key = zaf::Registry::CurrentUser().CreateSubKey(
        L"Software\\Classes\\.ra-bundle",
        zaf::RegistryRights::Write);

    file_type_key.SetStringValue({}, ProgID);
}

}

EntryResult RunRegisterEntry() {

    try {

        RegisterProgID();
        RegisterFileType();

        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nullptr, nullptr);

        return EntryResult::OK;
    }
    catch (const zaf::Error&) {
        return EntryResult::Fail;
    }
}

}