@echo off

del *.zip
rd /s /q .\build
del .\plmidi\*.pyd

.\venv\Scripts\python.exe setup.py build

for /d /r "build" %%d in (lib.*) do (
    copy %%d\*.pyd .\plmidi
)