@echo off

cd %~dp0

echo Build installer.
iscc ..\tool\installer\installer.iss
if errorlevel 1 (
	echo Build failed.
	goto end
)

:end