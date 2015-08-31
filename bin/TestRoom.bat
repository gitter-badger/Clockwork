@echo off
if exist "%~dp0ClockworkPlayer.exe" (set "DEBUG=") else (set "DEBUG=_d")
if exist "%~dp0..\share\Clockwork\Resources" (set "OPT1=-pp ..\share\Clockwork\Resources") else (set "OPT1=")
"%~dp0ClockworkPlayer%DEBUG%" Scripts/TestRoom.as %OPT1% %*
