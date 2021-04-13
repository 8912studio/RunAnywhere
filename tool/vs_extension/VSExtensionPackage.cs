using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Runtime.InteropServices;
using System.Threading;
using Task = System.Threading.Tasks.Task;

namespace VSExtension
{
    /// <summary>
    /// This is the class that implements the package exposed by this assembly.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The minimum requirement for a class to be considered a valid package for Visual Studio
    /// is to implement the IVsPackage interface and register itself with the shell.
    /// This package uses the helper classes defined inside the Managed Package Framework (MPF)
    /// to do it: it derives from the Package class that provides the implementation of the
    /// IVsPackage interface and uses the registration attributes defined in the framework to
    /// register itself and its components with the shell. These attributes tell the pkgdef creation
    /// utility what data to put into .pkgdef file.
    /// </para>
    /// <para>
    /// To get loaded into VS, the package must be referred by &lt;Asset Type="Microsoft.VisualStudio.VsPackage" ...&gt; in .vsixmanifest file.
    /// </para>
    /// </remarks>
    [PackageRegistration(UseManagedResourcesOnly = true, AllowsBackgroundLoading = true)]
    [ProvideAutoLoad(UIContextGuids.NoSolution, PackageAutoLoadFlags.BackgroundLoad)]
    [Guid(VSExtensionPackage.PackageGuidString)]
    public sealed class VSExtensionPackage : AsyncPackage
    {
        /// <summary>
        /// VSExtensionPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "95b0c241-8b50-4bb3-ae25-a32dbbff0a44";

        /// <summary>
        /// Initialization of the package; this method is called right after the package is sited, so this is the place
        /// where you can put all the initialization code that rely on services provided by VisualStudio.
        /// </summary>
        /// <param name="cancellationToken">A cancellation token to monitor for initialization cancellation, which can occur when VS is shutting down.</param>
        /// <param name="progress">A provider for progress updates.</param>
        /// <returns>A task representing the async work of package initialization, or an already completed task if there is none. Do not return null from this method.</returns>
        protected override async Task InitializeAsync(CancellationToken cancellationToken, IProgress<ServiceProgressData> progress)
        {
            // When initialized asynchronously, the current thread may be a background thread at this point.
            // Do any initialization that requires the UI thread after switching to the UI thread.
            await this.JoinableTaskFactory.SwitchToMainThreadAsync(cancellationToken);

            dte = await GetServiceAsync(typeof(EnvDTE.DTE)) as EnvDTE.DTE;

            var windowClass = new Win32.WNDCLASSEXW();
            windowClass.cbSize = Marshal.SizeOf(typeof(Win32.WNDCLASSEXW));
            windowClass.lpszClassName = "Zplutor.RunAnywhere.VS.Host";
            windowClass.lpfnWndProc = Marshal.GetFunctionPointerForDelegate(windowProcedure);

            ushort atom = Win32.RegisterClassExW(ref windowClass);
            hostWindowHandle = Win32.CreateWindowExW(
                0,
                atom,
                String.Empty,
                0,
                0,
                0,
                0,
                0,
                Win32.HWND_MESSAGE,
                IntPtr.Zero,
                IntPtr.Zero,
                IntPtr.Zero);
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            if (hostWindowHandle != IntPtr.Zero)
            {
                Win32.DestroyWindow(hostWindowHandle);
            }
        }

        private static IntPtr WindowProcedure(IntPtr hWnd, uint msg, IntPtr wParam, IntPtr lParam)
        {
            if (msg == Win32.WM_USER + 1)
            {
                string path = GetActiveDocumentPath();

                var copyDataStruct = new Win32.COPYDATASTRUCT();
                copyDataStruct.dwData = wParam;
                copyDataStruct.cbData = (uint)path.Length * 2;
                copyDataStruct.lpData = path;

                Win32.SendMessageW(wParam, Win32.WM_COPYDATA, IntPtr.Zero, ref copyDataStruct);
                return IntPtr.Zero;
            }

            return Win32.DefWindowProc(hWnd, msg, wParam, lParam);
        }

        private static string GetActiveDocumentPath()
        {
            if (dte == null)
            {
                return String.Empty;
            }

            if (dte.ActiveDocument == null)
            {
                return String.Empty;
            }

            return dte.ActiveDocument.FullName;
        }

        private static Win32.WndProc windowProcedure = WindowProcedure;
        private static EnvDTE.DTE dte;

        private IntPtr hostWindowHandle;
    }
}
