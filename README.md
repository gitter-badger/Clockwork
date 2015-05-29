#Clockwork Engine
 Clockwork is a 3D game engine build built by Sunwell studio, it is designed to be state of the art and rival professional grade engines. This is only in a Pre-Alpha state.

###Dependecies

    ###Windows
    	-June 2010 DirectX SDK(This is not necessary if building on Visual Studio 2012 or newer)

    ###Linux
    	Debian based
    	-libx11-dev
    	-libxrandr-dev
    	-libasound2-dev
    	-libgl1-mesa-dev

    	RedHat based
    	-libX11-devel
    	-libXrandr-devel
    	-alsa-lib-devel 
    	-mesa-libGL-devel

    ###Mac
    	-Xcode command line tools
    
    ###Androud
    	-Android SDK
    	-Android NDK (minimum API level 12) 

    ###Emscripten
    	-Emscripten SDK
    	-MinGW (On Windows)

    ###Raspberry Pi
    	Raspbian
    	-libasound2-dev
    	-libudev-dev

    	Pidora
    	-alsa-lib-devel
    	-systemd-devel



##How to build

Requires cmake

'''
<script-name> /path/to/build-tree [build-options]
'''



###Credit

Urho3D: providing a strong base for our engine. [Version we started from](https://github.com/urho3d/Urho3D/tree/c94bd4310b06e423b4f540e41da269d42fe09a2b)
