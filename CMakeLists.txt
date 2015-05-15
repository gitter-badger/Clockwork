cmake_minimum_required(VERSION 2.6.2)
set(CMAKE_ALLOW_LOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

if(CLOCKWORKENGINE_BUILD_PLATFORM_APPLE AND (CMAKE_VERSION VERSION_EQUAL 2.8.2) AND (CMAKE_GENERATOR STREQUAL "Xcode"))
	message(FATAL_ERROR "CMake 2.8.2 cannot create compatible Xcode projects for Xcode, please download the latest version or an older release from http://www.cmake.org/files/")
endif()

if(WIN32)
	set(CAMKE_USE_RELATIVE_PATHS true)
	set(CMAKE_SUPPRESS_REGENERATION true)
endif()

if(APPLE)
	if(CMAKE_VERSION VERSION_LESS 3.0.0)
		include(CMakeForceCompiler)
		CMAKE_FORCE_C_COMPILER(clang GNU)
		CMAKE_FORCE_CXX_COMPILER(clang++ GNU)
	endif()
	SET(CMAKE_SIZEOF_VOID_P 4)
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif ()

project(ClockworkEngine)


# add the -c and -Wall flags
add_definitions(
	-c
	-Wall
)

if ( CMAKE_BUILD_TYPE STREQUAL "" )
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS ${ClockworkEngine_SOURCE_DIR}/src/*.h)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS
	${ClockworkEngine_SOURCE_DIR}/src/*.cpp
	${ClockworkEngine_SOURCE_DIR}/src/*.c
)

add_executable(ClockworkEngine ${HDRS} ${SRCS})


SET(ClockworkEngine_CMAKE_DIR "${ClockworkEngine_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
INCLUDE(${ClockworkEngine_CMAKE_DIR}/Packages/FindGLEW.cmake)

INCLUDE(${ClockworkEngine_CMAKE_DIR}/Packages/FindGLFW.cmake)

if(WIN32)
	INCLUDE(${ClockworkEngine_CMAKE_DIR}/Packages/FindDirectX.cmake)
endif()

if(WIN32)
	include_directories(
		${ClockworkEngine_SOURCE_DIR}/headers
		${ClockworkEngine_SOURCE_DIR}/sources
		${GLEW_INCLUDE_DIRS}
		${GLFW_INCLUDE_DIRS}
		${DirectX_D3D11_INCLUDE_DIR} 
	)

	target_link_libraries( ClockworkEngine
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES}
		${DirectX_D3D11_LIBRARY}
	)
else()
	include_directories(
		${ClockworkEngine_SOURCE_DIR}/headers
		${ClockworkEngine_SOURCE_DIR}/sources
		${GLEW_INCLUDE_DIRS}
		${GLFW_INCLUDE_DIRS}
	)

	target_link_libraries( ClockworkEngine
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES}
	)
endif()

