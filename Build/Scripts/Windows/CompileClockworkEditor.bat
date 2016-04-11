call "%VS140COMNTOOLS%..\..\VC\bin\amd64\vcvars64.bat"
cmake ..\\..\\..\\ -DCLOCKWORK_DEV_BUILD=0 -G "Visual Studio 14 2015 Win64"
msbuild /m /p:Configuration=Release /p:Platform=x64 Source\ClockworkTool\GenerateScriptBindings.vcxproj
msbuild /m Clockwork.sln /t:ClockworkEditor /t:ClockworkPlayer /p:Configuration=Release /p:Platform=x64
