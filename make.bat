@echo off

if exist .\build (
    rd /s /q .\build
)
if exist .\*.pyd (
    del .\*.pyd
)

.\venv\Scripts\python.exe setup.py build

for /d /r "build" %%d in (lib.*) do (
    copy %%d\*.pyd .
)