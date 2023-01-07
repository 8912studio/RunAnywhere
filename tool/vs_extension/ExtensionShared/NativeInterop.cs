using System;
using System.Runtime.InteropServices;

namespace RunAnywhere
{
    class NativeInterop
    {
        public delegate void RequestFocusedPathCallback();

#if ForNewIDE
        [DllImport("Native_x64.dll",
            EntryPoint = "RunAnywhereVSHost_Initialize",
            CallingConvention = CallingConvention.StdCall)]
        public static extern void RunAnywhereVSHost_Initialize(
            IntPtr handle, 
            IntPtr request_event);

        [DllImport("Native_x64.dll",
            EntryPoint = "RunAnywhereVSHost_Uninitialize",
            CallingConvention = CallingConvention.StdCall)]
        public static extern void RunAnywhereVSHost_Uninitialize();
#else
        [DllImport("Native_x86.dll",
            EntryPoint = "RunAnywhereVSHost_Initialize",
            CallingConvention = CallingConvention.StdCall)]
        public static extern void RunAnywhereVSHost_Initialize(
            IntPtr handle,
            IntPtr request_event);

        [DllImport("Native_x86.dll",
            EntryPoint = "RunAnywhereVSHost_Uninitialize",
            CallingConvention = CallingConvention.StdCall)]
        public static extern void RunAnywhereVSHost_Uninitialize();
#endif
    }
}
