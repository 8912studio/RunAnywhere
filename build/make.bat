@echo off

cd %~dp0

for /f "delims=. tokens=1,2,3,4" %%a in (version) do (
	set major=%%a
	set minor=%%b
	set revision=%%c
	set /a build=%%d+1
)

set version=%major%.%minor%.%revision%.%build%

echo Current version is %version%
echo %version% > version

echo Update version of application.
echo #define MAJOR %major% > ..\src\version.h
echo #define MINOR %minor% >> ..\src\version.h
echo #define REVISION %revision% >> ..\src\version.h
echo #define BUILD %build% >> ..\src\version.h

echo Update version of installer.
echo #define MyAppVersion "%version%" > ..\tool\installer\version

echo ================================
echo Build zaf x64.
msbuild ..\third_party\zaf\zaf.sln -p:Configuration=Release-MT -p:Platform=x64
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build application.
msbuild ..\RunAnywhere.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build Visual Studio host extension.
msbuild ..\tool\vs_extension\VSExtension.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build Notepad++ host plugin x64.
msbuild ..\tool\npp_plugin\NPPPlugin.sln -p:Configuration=Release -p:Platform=x64
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build Notepad++ host plugin x86.
msbuild ..\tool\npp_plugin\NPPPlugin.sln -p:Configuration=Release -p:Platform=x86
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build VSCode host extension.
cd ..\tool\vscode_extension
call vsce package -o ..\..\build\out\Release\RunAnywhereVSCodeHost.vsix
if errorlevel 1 (
	cd %~dp0
	echo Build failed.
	goto end
)
cd %~dp0

call make_installer.bat
if errorlevel 1 (
	goto end
)

copy /y out\Release\RunAnywhere.pdb deploy\%version%
copy /y out\Release\InstallHelper.pdb deploy\%version%
copy /y out\Release\Native.pdb deploy\%version%
copy /y out\Release\RunAnywhereNPPHost_x64.pdb deploy\%version%
copy /y out\Release\RunAnywhereNPPHost_x86.pdb deploy\%version%
copy /y out\Release\RunAnywhereVSHost.pdb deploy\%version%

:end