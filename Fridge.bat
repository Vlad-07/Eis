@echo OFF
pushd %~dp0\
call Eis\vendor\premake\bin\premake5.exe vs2022
popd
PAUSE