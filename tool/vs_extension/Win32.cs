using System;
using System.Runtime.InteropServices;

namespace VSExtension
{
    class Win32
    {
        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        public struct WNDCLASSEXW
        {
            [MarshalAs(UnmanagedType.U4)]
            public int cbSize;
            [MarshalAs(UnmanagedType.U4)]
            public int style;
            public IntPtr lpfnWndProc;
            public int cbClsExtra;
            public int cbWndExtra;
            public IntPtr hInstance;
            public IntPtr hIcon;
            public IntPtr hCursor;
            public IntPtr hbrBackground;
            public string lpszMenuName;
            public string lpszClassName;
            public IntPtr hIconSm;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct COPYDATASTRUCT {
            public IntPtr dwData;
            public uint cbData;
            [MarshalAs(UnmanagedType.LPWStr)] 
            public string lpData;
        }

        public delegate IntPtr WndProc(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern ushort RegisterClassExW(ref WNDCLASSEXW lpWndClass);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern IntPtr CreateWindowExW(
           int dwExStyle,
           UInt16 regResult,
           string lpWindowName,
           UInt32 dwStyle,
           int x,
           int y,
           int nWidth,
           int nHeight,
           IntPtr hWndParent,
           IntPtr hMenu,
           IntPtr hInstance,
           IntPtr lpParam);

        [DllImport("user32.dll", SetLastError = true)]
        public static extern int DestroyWindow(IntPtr hwnd);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        public static extern int SendMessageW(IntPtr hwnd, uint message, IntPtr wParam, ref COPYDATASTRUCT lParam);

        [DllImport("user32.dll")]
        public static extern IntPtr DefWindowProc(IntPtr hWnd, uint uMsg, IntPtr wParam, IntPtr lParam);

        public static IntPtr HWND_MESSAGE = new IntPtr(-3);
        public const uint WM_COPYDATA = 0x4a;
        public const uint WM_USER = 0x400;
    }
}
