@echo off

if exist ".\release" (
    del /s /q .\release\*
) else (
    mkdir release
)
cl /P plmidi_cpp\setup.cpp
move setup.i .\release
ren .\release\setup.i release.i