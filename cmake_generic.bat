@echo off

:: Determine source tree and build tree
set "SOURCE=%~dp0"
set "BUILD="
if "%~1" == "" goto :continue
set "ARG1=%~1"
if "%ARG1:~0,1%" equ "-" goto :continue
set "BUILD=%~1"
shift
:continue
if "%BUILD%" == "" if exist "%cd%\CMakeCache.txt" (set "BUILD=%cd%") else (goto :error)

:: Detect CMake toolchains directory if it is not provided explicitly
if "%TOOLCHAINS%" == "" set "TOOLCHAINS=%SOURCE%/CMake/Toolchains"
if not exist "%TOOLCHAINS%" if exist "%CLOCKWORK_HOME%/share/Clockwork/CMake/Toolchains" set "TOOLCHAINS=%CLOCKWORK_HOME%/share/Clockwork/CMake/Toolchains"
:: BEWARE that the TOOLCHAINS variable leaks to caller's environment!

:: Default to native generator and toolchain if none is specified explicitly
set "OPTS="
set "arch="
if exist "%BUILD%\CMakeCache.txt" for /F "eol=/ delims=:= tokens=1-3" %%i in (%BUILD%\CMakeCache.txt) do if "%%i" == "CLOCKWORK_64BIT" if "%%k" == "1" set "arch= Win64"
:loop
if not "%~1" == "" (
    if "%~1" == "-DANDROID" if "%~2" == "1" set "OPTS=-G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAINS%\android.toolchain.cmake"
    if "%~1" == "-DEMSCRIPTEN" if "%~2" == "1" set "OPTS=-G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAINS%\emscripten.toolchain.cmake"
    if "%~1" == "-DCLOCKWORK_64BIT" if "%~2" == "1" set "arch= Win64"
    if "%~1" == "-DCLOCKWORK_64BIT" if "%~2" == "0" set "arch="
    if "%~1" == "-VS" set "OPTS=-G "Visual Studio %~2%arch%""
    shift
    shift
    goto loop
)

:: Create project with the chosen CMake generator and toolchain
cmake -E make_directory %BUILD% && cmake -E chdir %BUILD% cmake %OPTS% %* %SOURCE%

goto :eof
:error
echo Usage: %~nx0 \path\to\build-tree [build-options]
exit /B 1
:eof
