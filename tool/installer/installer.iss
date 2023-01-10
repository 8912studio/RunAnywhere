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
SetupMutex=Zplutor.RunAnywhereInstaller.Exclusive
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
PrivilegesRequiredOverridesAllowed=commandline
OutputBaseFilename={#MyAppName}-Installer-{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern 
WizardSizePercent=100,100
AlwaysShowDirOnReadyPage=yes
UsePreviousAppDir=yes
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
SourceDir=..\..\build\out\Release
OutputDir=..\..\deploy\{#MyAppVersion}
SetupIconFile=..\..\..\src\icon.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Components]
Name: "Main"; Description: "RunAnywhere main program"; Types: full compact custom; Flags: fixed
Name: "Addition"; Description: "Extensions for other applications"; Types: full; Flags: disablenouninstallwarning; Check: IsAdditionComponentVisible;
Name: "Addition\VSExtensionForOld"; Description: "Visual Studio extension for VS2019 and older"; Types: full; Flags: disablenouninstallwarning; Check: IsVS2019OrOlderInstalled;
Name: "Addition\VSExtensionForNew"; Description: "Visual Studio extension for VS2022 and newer"; Types: full; Flags: disablenouninstallwarning; Check: IsVS2022OrNewerInstalled;
Name: "Addition\VSCodeExtension"; Description: "Visual Studio Code extension"; Types: full; Flags: disablenouninstallwarning; Check: IsVSCodeInstalled;
Name: "Addition\NPPPlugin"; Description: "Notepad++ plugin"; Types: full; Flags: disablenouninstallwarning; Check: IsNPPInstalled;

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion; Components: "Main"
Source: "RunAnywhereVSHost_x64.vsix"; DestDir: "{app}\ExtensionsForOthers"; Components: "Main"
Source: "RunAnywhereVSHost_x86.vsix"; DestDir: "{app}\ExtensionsForOthers"; Components: "Main"
Source: "RunAnywhereVSCodeHost.vsix"; DestDir: "{app}\ExtensionsForOthers"; Components: "Main"
Source: "RunAnywhereNPPHost_x64.dll"; DestDir: "{app}\ExtensionsForOthers"; Flags: ignoreversion; Components: "Main"
Source: "RunAnywhereNPPHost_x86.dll"; DestDir: "{app}\ExtensionsForOthers"; Flags: ignoreversion; Components: "Main"
Source: "RunAnywhereNPPHost_x64.dll"; DestName: "RunAnywhereNPPHost.dll"; DestDir: "{code:NPPInstalledDirectoryPath}\plugins\RunAnywhereNPPHost"; Flags: ignoreversion; Components: "Addition\NPPPlugin"; Check: IsNPPX64;
Source: "RunAnywhereNPPHost_x86.dll"; DestName: "RunAnywhereNPPHost.dll"; DestDir: "{code:NPPInstalledDirectoryPath}\plugins\RunAnywhereNPPHost"; Flags: ignoreversion; Components: "Addition\NPPPlugin"; Check: IsNPPX86;
Source: "InstallHelper.dll"; Flags: dontcopy

[Run]
Filename: "code"; Parameters: "--install-extension ""{app}\ExtensionsForOthers\RunAnywhereVSCodeHost.vsix"""; Flags: shellexec runhidden waituntilterminated; Components: "Addition\VSCodeExtension"; Check: IsVSCodeInstalled;
Filename: "{code:VSIXInstallerPath}"; Parameters: """{app}\ExtensionsForOthers\RunAnywhereVSHost_x86.vsix"""; Flags: shellexec waituntilterminated; Components: "Addition\VSExtensionForOld"; Check: IsVS2019OrOlderInstalled;
Filename: "{code:VSIXInstallerPath}"; Parameters: """{app}\ExtensionsForOthers\RunAnywhereVSHost_x64.vsix"""; Flags: shellexec waituntilterminated; Components: "Addition\VSExtensionForNew"; Check: IsVS2022OrNewerInstalled;
Filename: "{app}\{#MyAppExeName}"; Parameters: "/register"; Description: "Register file associations"; Flags: postinstall
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]

//Visual Studio extension 
function External_IsVS2019OrOlderInstalled(): Integer;
external 'External_IsVS2019OrOlderInstalled@files:InstallHelper.dll cdecl';

function IsVS2019OrOlderInstalled(): Boolean;
begin
  Result := External_IsVS2019OrOlderInstalled() <> 0;
end;

function External_IsVS2022OrNewerInstalled(): Integer;
external 'External_IsVS2022OrNewerInstalled@files:InstallHelper.dll cdecl';

function IsVS2022OrNewerInstalled(): Boolean;
begin
  Result := External_IsVS2022OrNewerInstalled() <> 0;
end;

procedure External_GetVSIXInstallerPath(buffer: String; bufferLength: Integer);
external 'External_GetVSIXInstallerPath@files:InstallHelper.dll cdecl';

function VSIXInstallerPath(param: String): String;
begin
  SetLength(Result, 256);
  External_GetVSIXInstallerPath(Result, 256);
  Result := Copy(Result, 1 , Pos(#0, Result) - 1);
end;


//VSCode extension
function External_IsVSCodeInstalled(): Integer;
external 'External_IsVSCodeInstalled@files:InstallHelper.dll cdecl';

function IsVSCodeInstalled(): Boolean;
begin
  Result := External_IsVSCodeInstalled() <> 0;
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

function IsNPPInstalled(): Boolean;
var
  path: String;
begin
  GetNPPInstalledDirectoryPath(path);
  Result := (Length(path) <> 0);
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

procedure RegisterExtraCloseApplicationsResources();
var
  nppDirectoryPath: String;
begin
  if not WizardIsComponentSelected('Addition\NPPPlugin') then Exit;
  
  GetNPPInstalledDirectoryPath(nppDirectoryPath);
  if Length(nppDirectoryPath) = 0 then Exit;
  
  RegisterExtraCloseApplicationsResource(False, nppDirectoryPath + '\notepad++.exe');
end;

// All extensions
function IsAdditionComponentVisible(): Boolean;
begin
  Result := IsVS2019OrOlderInstalled() or IsVS2022OrNewerInstalled() or IsVSCodeInstalled() or IsNPPInstalled();
end;