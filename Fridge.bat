@echo OFF
pushd %~dp0\
git submodule update --remote
call Eis\vendor\.bin\premake\premake5.exe vs2022
popd
IF %ERRORLEVEL% NEQ 0 (
  PAUSE
)