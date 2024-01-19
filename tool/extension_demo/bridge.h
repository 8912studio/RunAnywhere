#pragma once

#include <run_anywhere_extension_sdk.h>

#ifdef __cplusplus
extern "C" {
#endif

RA_Command BridgeCreateCommand(const struct RA_CommandLine* command_line);
bool BridgeInterpret(RA_Command command, const struct RA_CommandLine* command_line);
const wchar_t* BridgeGetPreviewText(RA_Command command);
void BridgeDestroy(RA_Command command);

#ifdef __cplusplus
}
#endif