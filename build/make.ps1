Set-Location $PSScriptRoot

$date = Get-Date
Write-Output "Current date is $date"

$versionContent = Get-Content "version" -Raw
$versionParts = $versionContent -split '\.'

if ($versionParts.Length -ge 4) {
    $major = $versionParts[0]
    $minor = $versionParts[1]
    $revision = $versionParts[2]
    $build = [int]$versionParts[3] + 1
} else {
    Write-Error "Version file is not in the expected format."
    exit 1
}

$version = "$major.$minor.$revision.$build"

Write-Output "Current version is $version"
$version | Set-Content "version"

Write-Output "Update the version of application."
@"
#define MAJOR $major
#define MINOR $minor
#define REVISION $revision
#define BUILD $build
#define YEAR $($date.year)
#define MONTH $($date.month)
#define DAY $($date.day)
"@ | Set-Content "..\src\version.h"

Write-Output "Update the version of installer."
@"
#define MyAppVersion "$version"
#define MyAppYear "$($date.year)"
"@ | Set-Content "..\tool\installer\version"


function Build-Project {
    param (
        [string]$solutionPath,
        [string]$configuration,
        [string]$platform = ""
    )

    $platformArg = if ($platform) { "-p:Platform=$platform" } else { "" }
    
	msbuild "$solutionPath" -p:Configuration=$configuration $platformArg
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed."
        exit 1
    }
}

Write-Output "============================="
Write-Output "Build zaf x64."
Build-Project "..\third_party\zaf\zaf.sln" "Release-MT" "x64"

Write-Output "============================="
Write-Output "Build application."
Build-Project "..\RunAnywhere.sln" "Release"

Write-Output "============================="
Write-Output "Build Visual Studio host extension."
Build-Project "..\tool\vs_extension\VSExtension.sln" "Release"

Write-Output "============================="
Write-Output "Build Notepad++ host plugin x64."
Build-Project "..\tool\npp_plugin\NPPPlugin.sln" "Release" "x64"

Write-Output "============================="
Write-Output "Build Notepad++ host plugin x86."
Build-Project "..\tool\npp_plugin\NPPPlugin.sln" "Release" "x86"

Write-Output "============================="
Write-Output "Build VSCode host extension."
Set-Location "..\tool\vscode_extension"
$vsceCommand = "vsce package -o ..\..\build\out\Release\RunAnywhereVSCodeHost.vsix"
Invoke-Expression $vsceCommand
if ($LASTEXITCODE -ne 0) {
    Set-Location $PSScriptRoot
    Write-Error "Build failed for VSCode host extension"
    exit 1
}
Set-Location $PSScriptRoot

.\make_installer.bat
if ($LASTEXITCODE -ne 0) {
    Write-Error "Installer script failed"
    exit 1
}

$filesToCopy = @(
    "out\Release\RunAnywhere.pdb",
    "out\Release\InstallHelper.pdb",
    "out\Release\Native_x64.pdb",
    "out\Release\Native_x86.pdb",
    "out\Release\RunAnywhereNPPHost_x64.pdb",
    "out\Release\RunAnywhereNPPHost_x86.pdb",
    "out\Release\RunAnywhereVSHost_x64.pdb",
    "out\Release\RunAnywhereVSHost_x86.pdb"
)

foreach ($file in $filesToCopy) {
    $destination = "deploy\$version\$(Split-Path $file -Leaf)"
    Write-Output "Copying $file to $destination"
    Copy-Item $file -Destination $destination -Force
}
