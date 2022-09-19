#include "version"

#define MyAppName "RunAnywhere"
#define MyAppPublisher "Zplutor"
#define MyAppURL "https://github.com/Zplutor/RunAnywhere"
#define MyAppExeName "RunAnywhere.exe"

[Setup]
AppId={{F6182403-3960-4C49-BAEF-0231A9F574B6}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
PrivilegesRequiredOverridesAllowed=commandline
OutputBaseFilename={#MyAppName}-{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern 
WizardSizePercent=100,100
AlwaysShowDirOnReadyPage=yes
UsePreviousAppDir=yes
SourceDir=..\..\build\out\Release
OutputDir=..\..\deploy
SetupIconFile=..\..\..\src\icon.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "RunAnywhereVSHost.vsix"; DestDir: "{app}\ExtensionsForOthers"
Source: "RunAnywhereNPPHost_x64.dll"; DestDir: "{app}\ExtensionsForOthers"; Flags: ignoreversion
Source: "RunAnywhereNPPHost_x86.dll"; DestDir: "{app}\ExtensionsForOthers"; Flags: ignoreversion
Source: "RunAnywhereNPPHost_x64.dll"; DestName: "RunAnywhereNPPHost.dll"; DestDir: "{code:NPPInstalledDirectoryPath}\plugins\RunAnywhereNPPHost"; Flags: ignoreversion; Check: IsNPPX64
Source: "RunAnywhereNPPHost_x86.dll"; DestName: "RunAnywhereNPPHost.dll"; DestDir: "{code:NPPInstalledDirectoryPath}\plugins\RunAnywhereNPPHost"; Flags: ignoreversion; Check: IsNPPX86
Source: "InstallHelper.dll"; Flags: dontcopy

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Parameters: "/register"; Description: "Register file associations"; Flags: postinstall
Filename: "{app}\ExtensionsForOthers\RunAnywhereVSHost.vsix"; Flags: shellexec postinstall; Description: "Install extension for Visual Studio"; Check: IsVisualStudioInstalled
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]

//Visual Studio extension 
function External_IsVisualStudioInstalled(): Integer;
external 'External_IsVisualStudioInstalled@files:InstallHelper.dll cdecl';

function IsVisualStudioInstalled(): Boolean;
begin
  Result := External_IsVisualStudioInstalled() <> 0;
end;


//Notepad++ plugin
function External_GetNPPInstalledDirectoryPath(buffer: String; bufferLength: Integer): Integer;
external 'External_GetNPPInstalledDirectoryPath@files:InstallHelper.dll cdecl';

function GetNPPInstalledDirectoryPath(out path: String): Boolean;
begin
  SetLength(path, 256);
  Result := External_GetNPPInstalledDirectoryPath(path, 256) <> 0;
  path := Copy(path, 1 , Pos(#0, path) - 1);
end;

function NPPInstalledDirectoryPath(param: String): String;
begin
  GetNPPInstalledDirectoryPath(Result);
end;

function IsNPPX86(): Boolean;
var
  path: String;
  isX86: Boolean;
begin
  isX86 := GetNPPInstalledDirectoryPath(path);
  Result := (Length(path) <> 0) and isX86;
end;

function IsNPPX64(): Boolean;
var
  path: String;
  isX86: Boolean;
begin
  isX86 := GetNPPInstalledDirectoryPath(path);
  Result := (Length(path) <> 0) and (not isX86);
end;