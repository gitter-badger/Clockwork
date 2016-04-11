call "%VS140COMNTOOLS%..\..\VC\bin\amd64\vcvars64.bat"
cmake ..\\..\\  -DCLOCKWORK_DEV_BUILD=1 -G "Visual Studio 14 2015 Win64"
msbuild /m Clockwork.sln /t:ClockworkTool /p:Configuration=Release /p:Platform=x64
