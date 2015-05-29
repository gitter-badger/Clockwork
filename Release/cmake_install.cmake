# Install script for directory: /home/josh/Documents/Clockwork3D

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "/home/josh/Documents/Clockwork3D/bin/NinjaSnowWar.sh"
    "/home/josh/Documents/Clockwork3D/bin/Editor.sh"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Clockwork/Resources" TYPE DIRECTORY FILES
    "/home/josh/Documents/Clockwork3D/bin/CoreData"
    "/home/josh/Documents/Clockwork3D/bin/Data"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Clockwork/CMake" TYPE DIRECTORY FILES "/home/josh/Documents/Clockwork3D/CMake/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/Clockwork/Scripts" TYPE PROGRAM FILES
    "/home/josh/Documents/Clockwork3D/.bash_helpers.sh"
    "/home/josh/Documents/Clockwork3D/cmake_android.sh"
    "/home/josh/Documents/Clockwork3D/cmake_clean.sh"
    "/home/josh/Documents/Clockwork3D/cmake_mingw.sh"
    "/home/josh/Documents/Clockwork3D/cmake_macosx.sh"
    "/home/josh/Documents/Clockwork3D/cmake_emscripten.sh"
    "/home/josh/Documents/Clockwork3D/cmake_generic.sh"
    "/home/josh/Documents/Clockwork3D/cmake_eclipse.sh"
    "/home/josh/Documents/Clockwork3D/cmake_ios.sh"
    "/home/josh/Documents/Clockwork3D/cmake_codeblocks.sh"
    "/home/josh/Documents/Clockwork3D/cmake_rpi.sh"
    "/home/josh/Documents/Clockwork3D/cmake_ninja.sh"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/josh/Documents/Clockwork3D/Release/Source/cmake_install.cmake")
  include("/home/josh/Documents/Clockwork3D/Release/Docs/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/home/josh/Documents/Clockwork3D/Release/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/home/josh/Documents/Clockwork3D/Release/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
