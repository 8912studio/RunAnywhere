@echo off

cd %~dp0

echo ================================
echo Build install helper.
msbuild ..\tool\installer\helper\InstallHelper.sln -p:Configuration=Release
if errorlevel 1 (
	echo Build failed.
	goto end
)

echo ================================
echo Build installer.
iscc ..\tool\installer\installer.iss
if errorlevel 1 (
	echo Build failed.
	goto end
)

:end