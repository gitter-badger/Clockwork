
/usr/local/bin/mcs /out:../../../Artifacts/ClockworkNET/TPA/ClockworkNETBootstrap.dll /nostdlib /noconfig /t:library \
/lib:../../../Submodules/CoreCLR/MacOSX/Debug/x64/ \
/r:System.Console.dll /r:System.Runtime.dll /r:System.IO.dll /r:System.IO.FileSystem.dll /r:mscorlib.dll \
/r:System.Collections.dll /r:System.Collections.Concurrent.dll /r:System.Linq.dll \
ClockworkNETBootstrap/*.cs

/usr/local/bin/mcs /out:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll /nostdlib /noconfig /t:library /w:0 \
/lib:../../../Submodules/CoreCLR/MacOSX/Debug/x64/ \
/r:System.Console.dll /r:System.Runtime.dll /r:System.IO.dll /r:System.IO.FileSystem.dll /r:mscorlib.dll \
/r:System.Linq.dll \
../../../Build/Source/Generated/MACOSX/CSharp/Packages/Clockwork/Managed/*.cs \
../../../Build/Source/Generated/MACOSX/CSharp/Packages/ClockworkNET/Managed/*.cs \
../../../Build/Source/Generated/MACOSX/CSharp/Packages/ClockworkPlayer/Managed/*.cs \
ClockworkNETEngine/*.cs

/usr/local/bin/mcs /unsafe /out:../../../Artifacts/ClockworkNET/ClockworkNETTools.dll /nostdlib /noconfig /t:library /w:0 \
/lib:../../../Submodules/CoreCLR/MacOSX/Debug/x64/ \
/r:System.Console.dll /r:System.Runtime.dll /r:System.IO.dll /r:System.IO.FileSystem.dll /r:mscorlib.dll \
/r:System.Linq.dll /r:System.Reflection.Primitives.dll /r:System.Reflection.Metadata.dll \
/r:System.Collections.dll /r:System.Collections.Immutable.dll \
/r:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll \
ClockworkNETTools/*.cs

/usr/local/bin/mcs /out:../../../Artifacts/ClockworkNET/ClockworkNETTestCompile.dll /nostdlib /noconfig /t:library /w:0 \
/lib:../../../Submodules/CoreCLR/MacOSX/Debug/x64/ \
/r:System.Console.dll /r:System.Runtime.dll /r:System.IO.dll /r:System.IO.FileSystem.dll /r:mscorlib.dll \
/r:../../../Artifacts/ClockworkNET/ClockworkNETEngine.dll \
../ClockworkNETTest/MyClass.cs
