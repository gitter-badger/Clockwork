
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/rapidjson/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/kNet/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/FreeType/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/Box2D )


file (GLOB CSHARP_SOURCES *.cpp *.h)

if (NOT MSVC)
    # for kNet
    add_definitions (-DUNIX)
endif()

set (SOURCE_FILES ${CSHARP_SOURCES} ${CSHARP_BINDINGS_SOURCE} ${CSHARPCLOCKWORKPLAYER_BINDINGS_SOURCE})

add_library(ClockworkNETNative SHARED ${SOURCE_FILES})

target_link_libraries(ClockworkNETNative ClockworkJS ClockworkPlayerLib ClockworkPlayerJS NETCore ${CLOCKWORK_LINK_LIBRARIES})

if (APPLE)

target_link_libraries(ClockworkNETNative "-framework AudioUnit -framework Carbon -framework Cocoa -framework CoreAudio -framework ForceFeedback -framework IOKit -framework OpenGL -framework CoreServices -framework Security")

endif()
