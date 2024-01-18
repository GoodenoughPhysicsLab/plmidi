@echo off

rd /s /q .\build
del .\*.pyd

.\venv\Scripts\python.exe setup.py build

for /d /r "build" %%d in (lib.*) do (
    copy %%d\*.pyd .
)