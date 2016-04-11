
add_subdirectory(ThirdParty)
add_subdirectory(Clockwork)
add_subdirectory(ClockworkJS)


if (CLOCKWORK_WEBVIEW)
    add_subdirectory(ClockworkWebView)    
endif()

add_subdirectory(ClockworkPlayer)
add_subdirectory(ClockworkPlayerJS)

if (NOT IOS AND NOT ANDROID AND NOT EMSCRIPTEN)
    add_subdirectory(ToolCore)
    add_subdirectory(ToolCoreJS)
    add_subdirectory(ClockworkEditor)
    add_subdirectory(ClockworkTool)
    add_subdirectory(Tools)
    add_subdirectory(ClockworkNET)
endif()

include(ClockworkDocList)