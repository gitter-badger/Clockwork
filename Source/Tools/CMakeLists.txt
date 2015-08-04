

# Set project name
project (Clockwork-Tools)

# Find Clockwork library
find_package (Clockwork REQUIRED)
include_directories (${CLOCKWORK_INCLUDE_DIRS})

# ClockworkPlayer target is rather special, although it is here, it is not a tool because it is built into target platform binary as opposed to host platform
if (NOT EMSCRIPTEN)
    add_subdirectory (ClockworkPlayer)
endif ()

# Build PackageTool using host compiler toolchain
if ((CMAKE_CROSSCOMPILING OR IOS) AND CLOCKWORK_PACKAGING)
    # When cross-compiling, build the host tool as external project
    include (ExternalProject)
    if (IOS)
        # For iOS target, ensure the host environment is cleared first; Also workaround a known CMake/Xcode generator bug which prevents it from installing binaries correctly
        set (IOS_FIX CMAKE_COMMAND /usr/bin/env -i PATH=$ENV{PATH} ${CMAKE_COMMAND} BUILD_COMMAND bash -c "sed -i '' 's/EFFECTIVE_PLATFORM_NAME//g' CMakeScripts/install_postBuildPhase.make*")
    endif ()
    ExternalProject_Add (PackageTool
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source/Tools/PackageTool
        CMAKE_ARGS -DDEST_RUNTIME_DIR=${CMAKE_BINARY_DIR}/bin/tool -DBAKED_CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR} -DBAKED_CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR} ${IOS_FIX})
    install (PROGRAMS ${CMAKE_BINARY_DIR}/bin/tool/PackageTool DESTINATION ${DEST_RUNTIME_DIR}/tool)
endif ()

# All tools must be natively built and output to bin/tool subdir to differentiate them from target platfrom binaries in the bin directory
set_tool_output_directories ()

if (CLOCKWORK_TOOLS)
    # Clockwork tools
    add_subdirectory (AssetImporter)
    add_subdirectory (OgreImporter)
    add_subdirectory (PackageTool)
    add_subdirectory (RampGenerator)
    add_subdirectory (SpritePacker)
    if (CLOCKWORK_ANGELSCRIPT)
        add_subdirectory (ScriptCompiler)
    endif ()
elseif ((NOT CMAKE_CROSSCOMPILING AND NOT IOS) AND CLOCKWORK_PACKAGING)
    # PackageTool target is required but we are not cross-compiling, so build it as per normal
    add_subdirectory (PackageTool)
endif ()
