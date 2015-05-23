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
	)
endif()

find_package(OpenGL REQUIRED)
INCLUDE(${CMAKE_CMAKE_DIR}cmake/Packages/FindGLEW.cmake)
INCLUDE(${CMAKE_CMAKE_DIR}cmake/Packages/FindSDL2.cmake)

add_library(OpenGL4Renderer
	${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Renderer.cpp
	${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Renderer.h
	${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Window.cpp
	${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Window.h
)

add_library(Engine
	${CMAKE_SOURCE_DIR}/source/Clockwork/Clockwork.cpp
	${CMAKE_SOURCE_DIR}/source/Clockwork/Clockwork.h
)

add_executable(Test
  ${CMAKE_SOURCE_DIR}/source/Test/main.cpp
)

target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
target_include_directories(Core PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
target_link_libraries(Core PUBLIC Utilities)

if(WIN32)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h)
	target_include_directories(DirectX11Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IWindow.h)
	target_include_directories(DirectX11Renderer PUBLIC ${DirectX_D3D11_INCLUDE_DIR})
	target_link_libraries(DirectX11Renderer PUBLIC Utilities)
	target_link_libraries(DirectX11Renderer PUBLIC Core)
	target_link_libraries(DirectX11Renderer PUBLIC ${DirectX_D3D11_LIBRARY})
endif()

target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h)
target_include_directories(OpenGL4Renderer PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IWindow.h)
target_include_directories(OpenGL4Renderer PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(OpenGL4Renderer PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(OpenGL4Renderer PUBLIC Utilities)
target_link_libraries(OpenGL4Renderer PUBLIC Core)
target_link_libraries(OpenGL4Renderer PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(OpenGL4Renderer PUBLIC ${GLEW_LIBRARIES})
target_link_libraries(OpenGL4Renderer PUBLIC ${SDL2_LIBRARIES})

if(WIN32)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IWindow.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Renderer.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Window.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXRenderer.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/DirectX11Renderer/DXWindow.h)
	target_link_libraries(Engine PUBLIC Utilities)
	target_link_libraries(Engine PUBLIC DirectX11Renderer)
	target_link_libraries(Engine PUBLIC OpenGL4Renderer)
	target_link_libraries(Engine PUBLIC Core)
else()
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IWindow.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Renderer.h)
	target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/source/OpenGL4Renderer/GL4Window.h)
	target_link_libraries(Engine PUBLIC Utilities)
	target_link_libraries(Engine PUBLIC OpenGL4Renderer)
	target_link_libraries(Engine PUBLIC Core)
endif()

target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClEngineTypes.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMath.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClMatrix.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClPlatform.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClQuaternion.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Utilities/ClVector.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IRenderer.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Core/IWindow.h)
target_include_directories(Test PUBLIC ${CMAKE_SOURCE_DIR}/source/Clockwork/Clockwork.h)
target_link_libraries(Test PUBLIC Engine)
target_link_libraries(Test PUBLIC Core)
target_link_libraries(Test PUBLIC Utilities)
