

# Find Broadcom VideoCore firmware installation
#
#  BCM_VC_FOUND
#  BCM_VC_INCLUDE_DIRS
#  BCM_VC_LIBRARIES
#

if (NOT RPI)
    return ()
endif ()
if (BCM_VC_FOUND)
    return ()
endif ()
    
# Only need to cater for raspbian as they are not in CMAKE_SYSTEM_PATH
set (BCM_VC_INC_SEARCH_PATH /opt/vc/include)
set (BCM_VC_LIB_SEARCH_PATH /opt/vc/lib)

# Assume all the other headers are installed at same relative path as bcm_host.h
find_path (BCM_VC_INCLUDE_DIRS bcm_host.h PATHS ${BCM_VC_INC_SEARCH_PATH} PATH_SUFFIXES vc)

# Assume all the other libs are installed at the same relative path as libbcm_host.so
find_library (BCM_VC_LIB_BCM_HOST bcm_host PATHS ${BCM_VC_LIB_SEARCH_PATH} PATH_SUFFIXES vc)
find_library (BCM_VC_LIB_EGL EGL PATHS ${BCM_VC_LIB_SEARCH_PATH} PATH_SUFFIXES vc)
find_library (BCM_VC_LIB_GLES2 GLESv2 PATHS ${BCM_VC_LIB_SEARCH_PATH} PATH_SUFFIXES vc)

if (BCM_VC_INCLUDE_DIRS AND BCM_VC_LIB_BCM_HOST AND BCM_VC_LIB_EGL AND BCM_VC_LIB_GLES2)
    list (APPEND BCM_VC_INCLUDE_DIRS ${BCM_VC_INCLUDE_DIRS}/interface/vcos/pthreads)  # Note: variable change to list context after this
    set (BCM_VC_LIBRARIES ${BCM_VC_LIB_BCM_HOST} ${BCM_VC_LIB_EGL} ${BCM_VC_LIB_GLES2})
    set (BCM_VC_FOUND 1)
endif ()

if (BCM_VC_FOUND)
    include (FindPackageMessage)
    FIND_PACKAGE_MESSAGE (BCM_VC "Found Broadcom VideoCore firmware: ${BCM_VC_LIBRARIES} ${BCM_VC_INCLUDE_DIRS}" "[${BCM_VC_LIBRARIES}][${BCM_VC_INCLUDE_DIRS}]")
elseif (BCM_VC_FIND_REQUIRED)
    message (FATAL_ERROR "Could not find Broadcom VideoCore firmware")
endif ()

mark_as_advanced (BCM_VC_INCLUDE_DIRS BCM_VC_LIBRARIES BCM_VC_LIB_BCM_HOST BCM_VC_LIB_EGL BCM_VC_LIB_GLES2)
