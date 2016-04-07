@echo off
if exist "%~dp0ClockworkPlayer.exe" (set "DEBUG=") else (set "DEBUG=_d")
if exist "%~dp0..\share\Clockwork\Resources" (set "OPT1=-pp ..\share\Clockwork\Resources") else (set "OPT1=")
if [%1] == [] (set "OPT2=-w -s") else (set "OPT2=")
start "" "%~dp0ClockworkPlayer%DEBUG%" Scripts/Editor.as %OPT1% %OPT2% %*
