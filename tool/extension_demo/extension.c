#include <run_anywhere_extension_sdk.h>
#include "bridge.h"

/*
We use .c extension for this compile unit to ensure the SDK header file can be compiled in C 
compiler.
*/

RUN_ANYWHERE_EXPORT RA_Command __stdcall RA_Create(const struct RA_CommandLine* command_line) {
    return BridgeCreateCommand(command_line);
}


RUN_ANYWHERE_EXPORT bool __stdcall RA_Interpret(
    RA_Command command,
    const struct RA_CommandLine* command_line) {

    return BridgeInterpret(command, command_line);
}


RUN_ANYWHERE_EXPORT const wchar_t* __stdcall RA_GetPreviewText(RA_Command command) {
    return BridgeGetPreviewText(command);
}


RUN_ANYWHERE_EXPORT void __stdcall RA_Destroy(RA_Command command) {
    BridgeDestroy(command);
}