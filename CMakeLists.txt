cmake_minimum_required(VERSION 2.6)
project(ClockworkEngine)

add_library(Utilities
	${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.cpp
	${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h
	${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h
	${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h
	${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h
	${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h
	${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h
)

add_library(Core
	${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h
	${CMAKE_SOURCE_DIR}/source/Core/IWindow.h
)

if(WIN32)
	INCLUDE(${ClockworkEngine_CMAKE_DIR}cmake/Packages/FindDirectX.cmake)

	add_library(DirectX11Renderer
		${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXRenderer.cpp
		${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXRenderer.h
		${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXWindow.cpp
		${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXWindow.h
		${DirectX_D3D11_INCLUDE_DIR}
	)
endif()

find_package(OpenGL REQUIRED)
INCLUDE(${CMAKE_CMAKE_DIR}cmake/Packages/FindGLEW.cmake)
INCLUDE(${CMAKE_CMAKE_DIR}cmake/Packages/FindSDL2.cmake)

add_library(OpenGL3Renderer
	${CMAKE_SOURCE_DIR}/source/OpenGL3Renderer/GLRenderer.cpp
	${CMAKE_SOURCE_DIR}/source/OpenGL3Renderer/GLRenderer.h
	${CMAKE_SOURCE_DIR}/source/OpenGL3Renderer/GLWindow.cpp
	${CMAKE_SOURCE_DIR}/source/OpenGL3Renderer/GLWindow.h
	${GLEW_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
)

add_library(Engine
	${CMAKE_SOURCE_DIR}/source/Clockwork/Clockwork.cpp
	${CMAKE_SOURCE_DIR}/source/Clockwork/Clockwork.h
)

add_executable(Test
  ${CMAKE_SOURCE_DIR}/source/Test/main.cpp
)

target_link_libraries(Core PUBLIC Utilities)

if(WIN32)
	target_link_libraries(DirectX11Renderer PUBLIC Utilities Core ${DirectX_D3D11_LIBRARY})
endif()

target_link_libraries(OpenGL3Renderer PUBLIC Utilities Core ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARIES})

if(WIN32)
	target_link_libraries(Engine PUBLIC Utilities DirectX11Renderer OpenGL3Renderer Core)
else()
	target_link_libraries(Engine PUBLIC Utilities OpenGL3Renderer Core)
endif()

target_link_libraries(Test PUBLIC Engine)
