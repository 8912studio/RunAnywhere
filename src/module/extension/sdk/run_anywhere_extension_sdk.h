#pragma once

#ifdef RUN_ANYWHERE_EXTENSION_HOST
#define RUN_ANYWHERE_EXPORT
#else 
#define RUN_ANYWHERE_EXPORT __declspec(dllexport)
#endif

extern "C" {

struct RunAnywhere_CommandType {
    int dumb{};
};
typedef RunAnywhere_CommandType* RunAnywhere_Command;

RUN_ANYWHERE_EXPORT RunAnywhere_Command __stdcall RunAnywhere_Interpret(const wchar_t* text);

RUN_ANYWHERE_EXPORT const wchar_t* __stdcall RunAnywhere_GetText(RunAnywhere_Command command);

RUN_ANYWHERE_EXPORT void __stdcall RunAnywhere_Destroy(RunAnywhere_Command command);

}