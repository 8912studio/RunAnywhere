using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.IO;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;
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
    [PackageRegistration(UseManagedResourcesOnly = false, AllowsBackgroundLoading = true)]
    [ProvideAutoLoad(UIContextGuids.NoSolution, PackageAutoLoadFlags.BackgroundLoad)]
    [Guid(VSExtensionPackage.PackageGuidString)]
    public sealed class VSExtensionPackage : AsyncPackage
    {
        /// <summary>
        /// VSExtensionPackage GUID string.
        /// </summary>
        public const string PackageGuidString = "95b0c241-8b50-4bb3-ae25-a32dbbff0a44";

        private static Win32.RequestFocusedPathCallback requestFocusedPathCallback = OnRequestFocusedPathCallback;
        private static event Win32.RequestFocusedPathCallback requestFocusedPathEvent;

        private EnvDTE.DTE dte;
        private AnonymousPipeServerStream pipe;

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

            pipe = new AnonymousPipeServerStream(PipeDirection.Out);

            Win32.RunAnywhereVSHost_Initialize(
                pipe.ClientSafePipeHandle.DangerousGetHandle(),
                Marshal.GetFunctionPointerForDelegate(requestFocusedPathCallback));

            requestFocusedPathEvent += OnRequestFocusedPath;
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);

            Win32.RunAnywhereVSHost_Uninitialize();
        }

        private static void OnRequestFocusedPathCallback()
        {
            requestFocusedPathEvent.Invoke();
        }

        private void OnRequestFocusedPath()
        {
            SendBackFocusedPathAsync();
        }

        private async Task SendBackFocusedPathAsync()
        {
            await JoinableTaskFactory.SwitchToMainThreadAsync();

            string encoded_paths = GetEncodedPaths();

            byte[] length_buffer = BitConverter.GetBytes(encoded_paths.Length);
            pipe.Write(length_buffer, 0, length_buffer.Length);

            if (encoded_paths.Length != 0)
            {
                byte[] path_buffer = Encoding.Unicode.GetBytes(encoded_paths);
                pipe.Write(path_buffer, 0, path_buffer.Length);
            }
        }

        private string GetEncodedPaths()
        {
            string document_path = GetActiveDocumentPath();
            string solution_path = GetSolutionPath();

            if (document_path.Length == 0)
            {
                document_path = solution_path;
            }

            return document_path + '|' + solution_path;
        }

        private string GetActiveDocumentPath()
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

        private string GetSolutionPath()
        {
            if (dte == null)
            {
                return String.Empty;
            }

            if (dte.Solution == null)
            {
                return String.Empty;
            }

            return Path.GetDirectoryName(dte.Solution.FullName);
        }
    }
}
