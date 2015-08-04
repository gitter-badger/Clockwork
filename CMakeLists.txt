

# Set project name
project (Clockwork)

# Set minimum version
cmake_minimum_required (VERSION 2.8.6)

if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# Include Clockwork Cmake common module
include (Clockwork-CMake-common)

# Setup SDK install destinations
if (WIN32)
    set (SCRIPT_EXT .bat)
else ()
    set (SCRIPT_EXT .sh)
endif ()
set (PATH_SUFFIX Clockwork)
if (ANDROID)
    # For Android platform, install to a path similar to ANDROID_LIBRARY_OUTPUT_PATH variable, e.g. libs/armeabi-v7a
    set (LIB_SUFFIX s/${ANDROID_NDK_ABI_NAME})
elseif (CLOCKWORK_64BIT)
    if (EXISTS /usr/lib64)
        set (HAS_LIB64 TRUE)
    endif ()
    # Install to 'lib64' when one of these conditions is true
    if (WIN32 OR CLOCKWORK_USE_LIB64_RPM OR (HAS_LIB64 AND NOT CLOCKWORK_USE_LIB_DEB))
        set (LIB_SUFFIX 64)
    endif ()
endif ()
set (DEST_INCLUDE_DIR include/${PATH_SUFFIX})
set (DEST_SHARE_DIR share/${PATH_SUFFIX})
set (DEST_RUNTIME_DIR bin)
set (DEST_BUNDLE_DIR ${DEST_SHARE_DIR}/Applications)
# Note to package maintainer: ${PATH_SUFFIX} for library could be removed if the extra path is not desired, but if so then the RPATH setting in Source's CMakeLists.txt needs to be adjusted accordingly
set (DEST_LIBRARY_DIR lib${LIB_SUFFIX}/${PATH_SUFFIX})
# TODO: Investigate the implication of using CMAKE_SYSROOT variable (available since 3.0.0) instead of our own SYSROOT variable
set (SYSROOT ${ANDROID_TOOLCHAIN_ROOT} ${RPI_SYSROOT} ${MINGW_SYSROOT} ${IOS_SYSROOT} ${EMSCRIPTEN_SYSROOT} CACHE INTERNAL "Path to system root of the cross-compiling target")  # SYSROOT is empty for native build
set (DEST_PKGCONFIG_DIR lib${LIB_SUFFIX}/pkgconfig)
# Install application launcher scripts
file (GLOB APP_SCRIPTS ${CMAKE_SOURCE_DIR}/bin/*${SCRIPT_EXT})
install (PROGRAMS ${APP_SCRIPTS} DESTINATION ${DEST_RUNTIME_DIR})
# Install resource directories required by applications built with Clockwork library
install (DIRECTORY ${CMAKE_SOURCE_DIR}/bin/CoreData ${CMAKE_SOURCE_DIR}/bin/Data DESTINATION ${DEST_SHARE_DIR}/Resources)
# Install CMake modules and toolchains provided by and for Clockwork
install (DIRECTORY ${CMAKE_SOURCE_DIR}/CMake/ DESTINATION ${DEST_SHARE_DIR}/CMake)    # Note: the trailing slash is significant
# Install CMake build scripts
file (GLOB CMAKE_SCRIPTS ${CMAKE_SOURCE_DIR}/*${SCRIPT_EXT})
install (PROGRAMS ${CMAKE_SCRIPTS} DESTINATION ${DEST_SHARE_DIR}/Scripts)

# Setup package variables
set (CLOCKWORK_DESCRIPTION "Clockwork is a free lightweight, cross-platform 2D and 3D game engine implemented in C++ and released under the MIT license. Greatly inspired by OGRE (http://www.ogre3d.org) and Horde3D (http://www.horde3d.org).")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CLOCKWORK_DESCRIPTION})
set (CLOCKWORK_URL "https://github.com/clockwork3d/Clockwork")
set (CPACK_PACKAGE_VENDOR ${CLOCKWORK_URL})
set (CPACK_PACKAGE_CONTACT ${CLOCKWORK_URL})
execute_process (COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/CMake/Modules/GetClockworkRevision.cmake WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE CLOCKWORK_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
set (CPACK_PACKAGE_VERSION ${CLOCKWORK_VERSION})
string (REGEX MATCH "([^.]+)\\.([^.]+)\\.(.+)" MATCHED ${CLOCKWORK_VERSION})
if (MATCHED)
    set (CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_MATCH_1})
    set (CPACK_PACKAGE_VERSION_MINOR ${CMAKE_MATCH_2})
    set (CPACK_PACKAGE_VERSION_PATCH ${CMAKE_MATCH_3})
endif ()
set (CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/License.txt)
set (CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set (CPACK_GENERATOR TGZ)
if (ANDROID)
    set (CPACK_SYSTEM_NAME Android)
elseif (IOS)
    set (CPACK_SYSTEM_NAME iOS)
elseif (APPLE)
    set (CPACK_SYSTEM_NAME OSX)
elseif (WIN32)
    set (CPACK_GENERATOR ZIP)
elseif (EMSCRIPTEN)
    set (CPACK_SYSTEM_NAME HTML5)
elseif (CPACK_SYSTEM_NAME STREQUAL Linux)
    if (RPI)
        set (CPACK_SYSTEM_NAME Raspberry-Pi)
    endif ()
    if (NOT CLOCKWORK_64BIT OR HAS_LIB64)
        list (APPEND CPACK_GENERATOR RPM)
    endif ()
    if (NOT CLOCKWORK_64BIT OR NOT HAS_LIB64)
        list (APPEND CPACK_GENERATOR DEB)
    endif ()
    if (CLOCKWORK_64BIT)
        if (CLOCKWORK_USE_LIB64_RPM AND NOT HAS_LIB64)
            set (CPACK_GENERATOR RPM)
        elseif (CLOCKWORK_USE_LIB_DEB AND HAS_LIB64)
            set (CPACK_GENERATOR DEB)
        endif ()
    endif ()
    # Note to package maintainer: comment out below line to revert the prefix from '/usr/local' (default for CMAKE_INSTALL_PREFIX) to '/usr' (default for CPACK_PACKAGING_INSTALL_PREFIX)
    set (CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
endif ()
if (CLOCKWORK_64BIT)
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-64bit)
else ()
    set (CPACK_DEBIAN_PACKAGE_ARCHITECTURE i386)
    set (CPACK_RPM_PACKAGE_ARCHITECTURE i686)       # The 'package' target should be built with the help of 'setarch i686' command on a 64-bit host system
endif ()
set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-${CLOCKWORK_LIB_TYPE})
if (WIN32 AND NOT CLOCKWORK_OPENGL)
    if (CLOCKWORK_D3D11)
        set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-3D11)   # The artifact name has a space constraint on our website when viewed in a mobile browser, 3D11 stands for Direct3D 11
    else ()
        set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-3D9)
    endif ()
elseif (ANDROID AND X86)    # Take advantage of Android toolchain setting X86 variable to true for both 'x86' and 'x86_64' ABIs
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-IA)     # Stands for Intel Architecture
elseif (RPI AND RPI_ABI MATCHES ^armeabi-v7a)
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-v7a)
endif ()
if (NOT DEFINED ENV{RELEASE_TAG})
    set (CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-snapshot)
endif ()
include (CPack)

# Setup MacOSX and iOS bundle variables
if (IOS OR CLOCKWORK_MACOSX_BUNDLE)
    if (NOT MACOSX_BUNDLE_ICON_FILE)
        set (MACOSX_BUNDLE_ICON_FILE ClockworkIcon)
    endif ()
    if (NOT MACOSX_BUNDLE_BUNDLE_VERSION)
        set (MACOSX_BUNDLE_BUNDLE_VERSION ${CLOCKWORK_VERSION})
    endif ()
    if (NOT MACOSX_BUNDLE_LONG_VERSION_STRING)
        set (MACOSX_BUNDLE_LONG_VERSION_STRING ${CLOCKWORK_VERSION})
    endif ()
    if (NOT MACOSX_BUNDLE_SHORT_VERSION_STRING)
        set (MACOSX_BUNDLE_SHORT_VERSION_STRING ${CLOCKWORK_VERSION})
    endif ()
    if (NOT MACOSX_BUNDLE_COPYRIGHT)
        set (MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2008-2015 the Clockwork project.")
    endif ()
endif ()

# Setup SDK-like include dir in the build tree for building the Clockwork library
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DEST_INCLUDE_DIR}/ThirdParty)

# Clockwork source
add_subdirectory (Source)

# Clockwork documentation
add_subdirectory (Docs)
