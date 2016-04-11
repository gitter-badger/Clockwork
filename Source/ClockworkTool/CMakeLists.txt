set (CLOCKWORK_TOOL_SOURCES ClockworkTool.cpp ClockworkTool.h)

if (MSVC)
    add_definitions(-DCLOCKWORK_WIN32_CONSOLE)
endif(MSVC)

add_executable(ClockworkTool ${CLOCKWORK_TOOL_SOURCES})

target_link_libraries(ClockworkTool ToolCore NETCore NETScript Poco ${CLOCKWORK_LINK_LIBRARIES})

if (MSVC)
    target_link_libraries(ClockworkTool libcurl Iphlpapi Wldap32)

    # pre-Windows 8 can't count on D3DCompiler_47.dll being on system
    # TODO: Remove this dependency
    add_custom_command (TARGET ClockworkTool POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy_if_different \"${D3DCOMPILER_47_DLL}\" \"$<TARGET_FILE_DIR:ClockworkTool>/D3DCompiler_47.dll\")

else()
    target_link_libraries(ClockworkTool libcurl)
endif()

add_custom_command( TARGET ClockworkTool POST_BUILD
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -E make_directory ${CMAKE_SOURCE_DIR}/Artifacts/Build/ClockworkTool
                   COMMAND ${CMAKE_COMMAND}
                   ARGS -E copy_if_different $<TARGET_FILE:ClockworkTool> \"${CMAKE_SOURCE_DIR}/Artifacts/Build/ClockworkTool/\" )

if (MSVC)

# Copy the D3D shader compiler
add_custom_command( TARGET ClockworkTool POST_BUILD
                    COMMAND ${CMAKE_COMMAND}
                    ARGS -E copy_if_different \"${D3DCOMPILER_47_DLL}\" \"${CMAKE_SOURCE_DIR}/Artifacts/Build/ClockworkTool/D3DCompiler_47.dll\"
                    COMMENT "Copying ClockworkTool to Build Artifacts" )
endif(MSVC)

add_custom_target ( GenerateScriptBindings
                    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                    COMMAND ${CLOCKWORK_NODE_JAKE};build:genscripts[${JAVASCRIPT_BINDINGS_PLATFORM}]
                    DEPENDS ClockworkTool )

if (CLOCKWORK_DOTNET_DEV)

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/TPA)
 file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/TPA)
endif()

if (NOT EXISTS ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/Build)
 file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/Build)
endif()

if (MSVC)

add_custom_command(TARGET ClockworkTool POST_BUILD
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/Build/
                   COMMAND $<TARGET_FILE:ClockworkTool> net-projectgen "\"${CMAKE_SOURCE_DIR}/Script/ClockworkNET/ClockworkNETProjects.json\"" WINDOWS
                   COMMAND msbuild
                   COMMENT "Compiling ClockworkNET Assemblies")
elseif(APPLE)

if (NOT IOS)

# if we have xbuild available

if(EXISTS "/usr/local/bin/xbuild")

add_custom_command(TARGET ClockworkTool POST_BUILD
                   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Artifacts/ClockworkNET/Build/
                   COMMAND $<TARGET_FILE:ClockworkTool> net-projectgen "\"${CMAKE_SOURCE_DIR}/Script/ClockworkNET/ClockworkNETProjects.json\"" MACOSX
                   COMMAND /usr/local/bin/xbuild
                   COMMENT "Compiling ClockworkNET Assemblies")
endif(xbuild)
endif(NOT IOS)
endif()

endif(CLOCKWORK_DOTNET_DEV)
