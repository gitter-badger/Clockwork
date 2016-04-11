call "%VS140COMNTOOLS%..\..\VC\bin\amd64\vcvars64.bat"
cmake %1 -DCLOCKWORK_DEV_BUILD=1 -G "Visual Studio 14 2015 Win64"
msbuild Clockwork.sln /m /p:Configuration=Debug /p:Platform=x64 /t:ClockworkTool
