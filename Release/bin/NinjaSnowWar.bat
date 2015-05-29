:: To avoid cacophony of multiple background music when testing with multiple connections on the same test machine,
:: start all the other non-headless connections with '-nobgm' argument.
:: E.g. 1 - headless server
::   Start the server with "NinjaSnowWar -headless -server"
::   Start the first client with "NinjaSnowWar -w -address <put-your-host-name-here>"
::   Start the subsequent clients on the same host with "NinjaSnowWar -w -nobgm -address <put-your-host-name-here>"
::
:: E.g. 2 - non-headless server
::   Start the server with "NinjaSnowWar -w -server"
::   Start the client on the same host with "NinjaSnowWar -w -nobgm -address <put-your-host-name-here>"
::
@echo off
if exist "%~dp0ClockworkPlayer.exe" (set "DEBUG=") else (set "DEBUG=_d")
if exist "%~dp0..\share\Clockwork\Resources" (set "OPT1=-pp ..\share\Clockwork\Resources") else (set "OPT1=")
"%~dp0ClockworkPlayer%DEBUG%" Scripts/NinjaSnowWar.as %OPT1% %*
