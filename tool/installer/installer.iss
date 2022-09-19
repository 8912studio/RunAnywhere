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
Source: "RunAnywhereNPPHost.dll"; DestDir: "{app}\ExtensionsForOthers"
Source: "RunAnywhereNPPHost.dll"; DestDir: "{code:GetNPPInstalledDirectoryPath}\plugins\RunAnywhereNPPHost"
Source: "InstallHelper.dll"; Flags: dontcopy

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Parameters: "/register"; Description: "Register file associations"; Flags: postinstall
Filename: "{app}\ExtensionsForOthers\RunAnywhereVSHost.vsix"; Flags: shellexec postinstall; Description: "Install extension for Visual Studio"; Check: CheckIfShowVSExtension
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function External_IsVisualStudioInstalled(): Integer;
external 'External_IsVisualStudioInstalled@files:InstallHelper.dll cdecl';

function External_GetNPPInstalledDirectoryPath(buffer: String; bufferLength: Integer): Integer;
external 'External_GetNPPInstalledDirectoryPath@files:InstallHelper.dll cdecl';

function CheckIfShowVSExtension(): Boolean;
begin
  Result := External_IsVisualStudioInstalled() <> 0;
end;

function GetNPPInstalledDirectoryPath(param: String): String;
var
  path: String;
  pathLength: Integer;
begin
  SetLength(path, 256); 
  pathLength := External_GetNPPInstalledDirectoryPath(path, 256); 
  Result := Copy(path, 1 , pathLength);
end;