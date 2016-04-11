
include(ClockworkCommon)

set (JAVASCRIPT_BINDINGS_PLATFORM "WEB")

add_definitions(-DCLOCKWORK_PLATFORM_WEB)
add_definitions(-DCLOCKWORK_OPENGL -Wno-warn-absolute-paths -DCLOCKWORK_TBUI)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof -std=gnu++0x")

set (CLOCKWORK_LINK_LIBRARIES ${CLOCKWORK_LINK_LIBRARIES})
