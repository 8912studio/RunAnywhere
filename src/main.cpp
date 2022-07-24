#include <Windows.h>
#include "entry/main_entry.h"

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR command_line, int) {

    auto result = ra::entry::RunMainEntry(command_line);
    return static_cast<int>(result);
}