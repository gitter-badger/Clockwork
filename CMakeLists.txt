
project (Clockwork)

cmake_minimum_required (VERSION 2.8)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Build/CMake/Modules)

include(ClockworkUtils)

add_definitions(-DCLOCKWORK_ROOT_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DCLOCKWORK_ROOT_BUILD_DIR="${CMAKE_BINARY_DIR}")

add_definitions( -DCLOCKWORK_API= -DCLOCKWORK_STATIC_DEFINE -DCLOCKWORK_CLOCKWORK2D -DCLOCKWORK_LOGGING -DCLOCKWORK_PROFILING)

if (NOT DEFINED CLOCKWORK_DEV_BUILD)
    set(CLOCKWORK_DEV_BUILD 1)
ENDIF()

if (CLOCKWORK_DEV_BUILD)
    add_definitions("-DCLOCKWORK_DEV_BUILD=1")
endif()

# Disable until https://github.com/ClockworkGameEngine/ClockworkGameEngine/issues/554 is addressed
# add_definitions("-DCLOCKWORK_SOURCE_BUILD=1")

# this is here as QtCreator is having trouble picking up #include <Clockwork/*> without it
include_directories(${CMAKE_SOURCE_DIR}/Source ${CMAKE_SOURCE_DIR}/Source/ClockworkEditor/Source)

set (CLOCKWORK_LINK_LIBRARIES Clockwork Box2D Duktape TurboBadger FreeType JO LZ4 PugiXml STB)

if (NOT CLOCKWORK_BUILD_2D)
    add_definitions( -DCLOCKWORK_PHYSICS -DCLOCKWORK_3D)
    set (CLOCKWORK_LINK_LIBRARIES ${CLOCKWORK_LINK_LIBRARIES} Bullet StanHull)
endif()

if (NOT EMSCRIPTEN)
    add_definitions( -DCLOCKWORK_NETWORK)
    set (CLOCKWORK_LINK_LIBRARIES ${CLOCKWORK_LINK_LIBRARIES} SDL Civetweb Recast Detour DetourCrowd DetourTileCache kNet libcurl )
endif()

add_definitions( -DCLOCKWORK_WEB )

if (MSVC)

    include(ClockworkWindows)

elseif(APPLE)

    if (IOS)
        include(ClockworkIOS)
    else()
        include(ClockworkMac)
    endif()

elseif(LINUX)

  include(ClockworkLinux)

elseif(ANDROID)

    include(ClockworkAndroid)

elseif(EMSCRIPTEN)

    include(ClockworkWeb)

endif()

find_program(CLDOC cldoc)
if(CLDOC)
   add_custom_target(docs DEPENDS ClockworkEngineDocs)
endif()

if (CLOCKWORK_WEBVIEW)
  if(APPLE)
    if(POLICY CMP0037)
      # new cmake doesn't like creating framework whose name has spaces
      # which CEF3 scripts (including shell) currently require on OSX
      cmake_policy(SET CMP0037 OLD)
    endif()
    include_directories (${CMAKE_SOURCE_DIR}/Submodules/CEF/MacOSX/)
    add_subdirectory(Submodules/CEF/MacOSX/libcef_dll)
  elseif(MSVC)
    if (CLOCKWORK_PROJECT_ARCH STREQUAL "x86")
        include_directories (${CMAKE_SOURCE_DIR}/Submodules/CEF/Windows/32bit)
        add_subdirectory(Submodules/CEF/Windows/32bit/libcef_dll)
    else()
        include_directories (${CMAKE_SOURCE_DIR}/Submodules/CEF/Windows/64bit)
        add_subdirectory(Submodules/CEF/Windows/64bit/libcef_dll)
    endif()

  else()
    include_directories (${CMAKE_SOURCE_DIR}/Submodules/CEF/Linux)
    add_subdirectory(Submodules/CEF/Linux/libcef_dll)
  endif()
endif()

add_subdirectory(Source)
