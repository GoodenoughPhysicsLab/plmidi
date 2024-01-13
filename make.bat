@echo off

rd /s /q .\build

.\venv\Scripts\python.exe plmidi_setup.py build

for /d /r "build" %%d in (lib.*) do (
    copy %%d\*.pyd .\venv\Lib\site-packages
)