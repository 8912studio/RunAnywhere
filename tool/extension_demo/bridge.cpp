#include "bridge.h"
#include <cwchar>
#include "extension_command.h"

RA_Command BridgeCreateCommand(const RA_CommandLine* command_line) {

    if (std::wcscmp(command_line->command, L"extension") == 0) {
        return reinterpret_cast<RA_Command>(new ExtensionCommand());
    }

    return nullptr;
}


bool BridgeInterpret(RA_Command command, const RA_CommandLine* command_line) {

    auto extension_command = reinterpret_cast<ExtensionCommand*>(command);
    return extension_command->Interpret(command_line);
}


const wchar_t* BridgeGetPreviewText(RA_Command command) {

    auto extension_command = reinterpret_cast<ExtensionCommand*>(command);
    return extension_command->GetText().c_str();
}


void BridgeDestroy(RA_Command command) {

    auto extension_command = reinterpret_cast<ExtensionCommand*>(command);
    delete extension_command;
}
