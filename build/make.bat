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

echo Build zaf.
msbuild ..\third_party\zaf\zaf.sln -p:Configuration=Release-MT -p:PlatformTarget=x64
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build application.
msbuild ..\RunAnywhere.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build Visual Studio host extension.
msbuild ..\tool\vs_extension\VSExtension.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build Notepad++ host plugin x64.
msbuild ..\tool\npp_plugin\NPPPlugin.sln -p:Configuration=Release -p:Platform=x64
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build Notepad++ host plugin x86.
msbuild ..\tool\npp_plugin\NPPPlugin.sln -p:Configuration=Release -p:Platform=x86
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build install helper.
msbuild ..\tool\installer\helper\InstallHelper.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo Build installer.
iscc ..\tool\installer\installer.iss
if errorlevel 1 (
	echo Build failed.
	goto end
)

:end