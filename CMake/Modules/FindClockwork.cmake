

# Find Clockwork include directories and library in source & build tree or installed Clockwork SDK.
#
#  CLOCKWORK_FOUND
#  CLOCKWORK_INCLUDE_DIRS
#  CLOCKWORK_LIBRARIES
#  CLOCKWORK_LIBRARIES_REL (WIN32 only)
#  CLOCKWORK_LIBRARIES_DBG (WIN32 Only)
#
#
# For internal Clockwork project, the Clockwork "build tree" path is already known.
#
# For external project that attempts to use the Clockwork build tree or installed Clockwork SDK,
# use CLOCKWORK_HOME environment variable or build option to specify the path (not needed when the path is a system-wide default location).
# When setting CLOCKWORK_HOME variable, it should be set to a parent directory containing both the "include" or "lib" subdirectories.
# For example: set CLOCKWORK_HOME=/home/john/usr/local, if the SDK is installed using DESTDIR=/home/john and CMAKE_INSTALL_PREFIX=/usr/local
#

if (CLOCKWORK_FOUND)
    # All the subprojects should use the same Clockwork library, so only need to search on the first (sub)project that requires Clockwork library
    return ()
endif ()

# If the CLOCKWORK_LIB_TYPE build option changes then invalidate the found library cache
if (NOT CLOCKWORK_LIB_TYPE STREQUAL CLOCKWORK_FOUND_LIB_TYPE)
    unset (CLOCKWORK_LIBRARIES CACHE)
    set (CLOCKWORK_FOUND_LIB_TYPE ${CLOCKWORK_LIB_TYPE} CACHE INTERNAL "Lib type when Clockwork library was last found")

    # Clockwork prefers static library type by default while CMake prefers shared one, so we need to change CMake preference to agree with Clockwork
    if (NOT CLOCKWORK_LIB_TYPE STREQUAL SHARED)
        list (REVERSE CMAKE_FIND_LIBRARY_SUFFIXES)
    endif ()
endif ()

set (PATH_SUFFIX Clockwork)
if (CMAKE_PROJECT_NAME STREQUAL Clockwork AND TARGET Clockwork)
    # Library location is already known to be in the build tree
    set (CLOCKWORK_HOME ${CMAKE_BINARY_DIR})
    set (CLOCKWORK_INCLUDE_DIRS ${CLOCKWORK_HOME}/include ${CLOCKWORK_HOME}/include/${PATH_SUFFIX}/ThirdParty)
    if (CLOCKWORK_PHYSICS)
        # Bullet library depends on its own include dir to be added in the header search path
        # This is more practical than patching its header files in many places to make them work with relative path
        list (APPEND CLOCKWORK_INCLUDE_DIRS ${CLOCKWORK_HOME}/include/${PATH_SUFFIX}/ThirdParty/Bullet)
    endif ()
    if (CLOCKWORK_LUA)
        # ditto for Lua/LuaJIT
        list (APPEND CLOCKWORK_INCLUDE_DIRS ${CLOCKWORK_HOME}/include/${PATH_SUFFIX}/ThirdParty/Lua${JIT})
    endif ()
    set (CLOCKWORK_LIBRARIES Clockwork)
    set (FOUND_MESSAGE "Found Clockwork: as CMake target")
else ()
    # Library location would be searched (based on CLOCKWORK_HOME variable if provided and in system-wide default location)
    if (NOT CLOCKWORK_HOME AND DEFINED ENV{CLOCKWORK_HOME})
        file (TO_CMAKE_PATH "$ENV{CLOCKWORK_HOME}" CLOCKWORK_HOME)
    endif ()
    # CLOCKWORK_HOME variable should be an absolute path, so use a non-rooted search even when we are cross-compiling
    if (CLOCKWORK_HOME)
        list (APPEND CMAKE_PREFIX_PATH ${CLOCKWORK_HOME})
        set (SEARCH_OPT NO_CMAKE_FIND_ROOT_PATH)
    endif ()
    # For Android platform, search in path similar to ANDROID_LIBRARY_OUTPUT_PATH variable
    if (ANDROID)
        if (CLOCKWORK_HOME)
            set (CLOCKWORK_LIB_SEARCH_HINT HINTS ${CLOCKWORK_HOME}/libs/${ANDROID_NDK_ABI_NAME})
        else ()
            set (CLOCKWORK_LIB_SEARCH_HINT HINTS /usr/local/libs/${ANDROID_NDK_ABI_NAME})
        endif ()
    endif ()
    if (NOT CLOCKWORK_64BIT)
        # For 32-bit, force to search in 'lib' path even when the host system defaulted to use 'lib64'
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS FALSE)
    elseif (WIN32)
        # For 64-bit, force to search in 'lib64' path even when the Windows platform is not defaulted to use it
        set_property (GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)
    endif ()
    find_path (CLOCKWORK_INCLUDE_DIRS Clockwork.h PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    find_library (CLOCKWORK_LIBRARIES NAMES Clockwork ${CLOCKWORK_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
    if (WIN32)
        # For Windows platform, give a second chance to search for a debug version of the library
        find_library (CLOCKWORK_LIBRARIES_DBG NAMES Clockwork_d ${CLOCKWORK_LIB_SEARCH_HINT} PATH_SUFFIXES ${PATH_SUFFIX} ${SEARCH_OPT})
        # If both the non-debug and debug version of the libraries are found then use them both
        set (CLOCKWORK_LIBRARIES_REL ${CLOCKWORK_LIBRARIES})
        # Otherwise, CLOCKWORK_LIBRARIES variable should have the path to either one of the version
        if (CLOCKWORK_LIBRARIES)
            if (CLOCKWORK_LIBRARIES_DBG)
                list (APPEND CLOCKWORK_LIBRARIES ${CLOCKWORK_LIBRARIES_DBG})
            endif ()
        else ()
            set (CLOCKWORK_LIBRARIES ${CLOCKWORK_LIBRARIES_DBG})
        endif ()
    endif ()
    if (CLOCKWORK_INCLUDE_DIRS)
        set (BASE_INCLUDE_DIR ${CLOCKWORK_INCLUDE_DIRS})   # Preserve the base include dir because the original variable will be turned into a list below
        get_filename_component (PATH ${CLOCKWORK_INCLUDE_DIRS} PATH)
        set (CLOCKWORK_INCLUDE_DIRS ${PATH} ${CLOCKWORK_INCLUDE_DIRS}/ThirdParty)
        if (CLOCKWORK_PHYSICS)
            list (APPEND CLOCKWORK_INCLUDE_DIRS ${BASE_INCLUDE_DIR}/ThirdParty/Bullet)
        endif ()
        if (CLOCKWORK_LUA)
            list (APPEND CLOCKWORK_INCLUDE_DIRS ${BASE_INCLUDE_DIR}/ThirdParty/Lua${JIT})
        endif ()
        if (NOT CLOCKWORK_HOME)
            # CLOCKWORK_HOME is not set when using SDK installed on system-wide default location, so set it now
            get_filename_component (PATH ${PATH} PATH)
            set (CLOCKWORK_HOME ${PATH})
        endif ()
    endif ()
endif ()

if (CLOCKWORK_INCLUDE_DIRS AND CLOCKWORK_LIBRARIES)
    set (CLOCKWORK_FOUND 1)
    if (NOT FOUND_MESSAGE)
        set (FOUND_MESSAGE "Found Clockwork: ${CLOCKWORK_LIBRARIES}")
    endif ()
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (Clockwork ${FOUND_MESSAGE} "[${CLOCKWORK_LIBRARIES}][${CLOCKWORK_INCLUDE_DIRS}]")
    set (CLOCKWORK_HOME ${CLOCKWORK_HOME} CACHE PATH "Path to Clockwork build tree or SDK installation location (external project only)" FORCE)
elseif (Clockwork_FIND_REQUIRED)
    if (ANDROID)
        set (NOT_FOUND_MESSAGE "For Android platform, double check if you have specified to use the same ANDROID_ABI as the Clockwork library in the build tree or SDK.")
    endif ()
    message (FATAL_ERROR
        "Could not find Clockwork library in Clockwork build tree or SDK installation. "
        "Use CLOCKWORK_HOME environment variable or build option to specify the location of the build tree or SDK installation. ${NOT_FOUND_MESSAGE}")
endif ()

mark_as_advanced (CLOCKWORK_INCLUDE_DIRS CLOCKWORK_LIBRARIES CLOCKWORK_LIBRARIES_REL CLOCKWORK_LIBRARIES_DBG CLOCKWORK_HOME)
