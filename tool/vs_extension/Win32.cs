using System;
using System.Runtime.InteropServices;

namespace VSExtension
{
    class Win32
    {
        public delegate void RequestFocusedPathCallback();

        [DllImport("Native.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void RunAnywhereVSHost_Initialize(IntPtr handle, IntPtr request_event);

        [DllImport("Native.dll")]
        public static extern void RunAnywhereVSHost_Uninitialize();
    }
}
