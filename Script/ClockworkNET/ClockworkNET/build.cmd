
csc /out:../../../Artifacts/ClockworkNET/TPA/ClockworkNETBootstrap.dll /nostdlib+ /noconfig /t:library ^
/lib:../../../Submodules/CoreCLR/Windows/Release/x64/ ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Collections.Concurrent.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Runtime.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Linq.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Collections.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/mscorlib.ni.dll ^
ClockworkNETBootstrap/*.cs

csc /out:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll /nostdlib+ /noconfig /t:library /w:0 ^
/lib:../../../Submodules/CoreCLR/Windows/Release/x64/ ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Runtime.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.IO.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.IO.FileSystem.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Linq.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/mscorlib.ni.dll ^
../../../Build/Source/Generated/WINDOWS/CSharp/Packages/Clockwork/Managed/*.cs ^
../../../Build/Source/Generated/WINDOWS/CSharp/Packages/ClockworkNET/Managed/*.cs ^
../../../Build/Source/Generated/WINDOWS/CSharp/Packages/ClockworkPlayer/Managed/*.cs ^
ClockworkNETEngine/*.cs

csc /unsafe /out:../../../Artifacts/ClockworkNET/ClockworkNETTools.dll /nostdlib+ /noconfig /t:library /w:0 ^
/lib:../../../Submodules/CoreCLR/Windows/Release/x64/ ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Runtime.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.IO.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Linq.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Reflection.Primitives.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Reflection.Metadata.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Collections.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Collections.Immutable.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/mscorlib.ni.dll ^
/r:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll ^
ClockworkNETTools/*.cs

csc /out:../../../Artifacts/ClockworkNET/ClockworkNETTestCompile.dll ^
/debug+ /nostdlib+ /noconfig /t:library /w:0 ^
/lib:../../../Submodules/CoreCLR/Windows/Release/x64/ ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.Runtime.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.IO.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/System.IO.FileSystem.dll ^
/r:../../../Submodules/CoreCLR/Windows/Release/x64/mscorlib.ni.dll ^
/r:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll ^
../ClockworkNETTest/MyClass.cs
