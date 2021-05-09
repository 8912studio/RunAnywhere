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
Source: "InstallHelper.dll"; Flags: dontcopy

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\ExtensionsForOthers\RunAnywhereVSHost.vsix"; Flags: shellexec postinstall; Description: "Install extension for Visual Studio"; Check: CheckIfShowVSExtension
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function IsVisualStudioInstalled(): Integer;
external 'IsVisualStudioInstalled@files:InstallHelper.dll cdecl';

function CheckIfShowVSExtension(): Boolean;
begin
  Result := IsVisualStudioInstalled() <> 0;
end;