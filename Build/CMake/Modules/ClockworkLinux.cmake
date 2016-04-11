set (JAVASCRIPT_BINDINGS_PLATFORM "LINUX")
set (CLOCKWORK_NODE_JAKE Build/Linux/node/node Build/node_modules/jake/bin/cli.js  -f  Build/Scripts/Bootstrap.js)

include(ClockworkDesktop)

add_definitions(-DCLOCKWORK_PLATFORM_LINUX -DCLOCKWORK_OPENGL -DKNET_UNIX -DHAVE_INT64_T)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -std=gnu++0x")

find_package(PkgConfig REQUIRED)

#for native file dialog
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

list (APPEND CLOCKWORK_LINK_LIBRARIES pthread GLEW GL dl)


add_definitions(-DCLOCKWORK_PLATFORM_LINUX)
add_definitions(-DCLOCKWORK_OPENGL -Wno-warn-absolute-paths -DCLOCKWORK_TBUI)
