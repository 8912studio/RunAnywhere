#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef RUN_ANYWHERE_EXTENSION_HOST
#define RUN_ANYWHERE_EXPORT
#else 
#define RUN_ANYWHERE_EXPORT __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct RA_CommandType {
    int dumb;
};
typedef struct RA_CommandType* RA_Command;

enum RA_CommandLinePieceType {
    RA_NormalText,
    RA_TextBlock,
    RA_ActivePath,
};

struct RA_CommandLinePiece {
    enum RA_CommandLinePieceType type;
    const wchar_t* content;
};

struct RA_CommandLine {
    const wchar_t* command;
    const struct RA_CommandLinePiece* arguments;
    int32_t argument_count;
};

RUN_ANYWHERE_EXPORT RA_Command __stdcall RA_Create(const struct RA_CommandLine* command_line);

RUN_ANYWHERE_EXPORT bool __stdcall RA_Interpret(
    RA_Command command, 
    const struct RA_CommandLine* command_line);

RUN_ANYWHERE_EXPORT const wchar_t* __stdcall RA_GetPreviewText(RA_Command command);

RUN_ANYWHERE_EXPORT void __stdcall RA_Destroy(RA_Command command);

#ifdef __cplusplus
}
#endif