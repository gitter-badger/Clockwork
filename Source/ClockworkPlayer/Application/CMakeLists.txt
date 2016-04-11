
include_directories(${CMAKE_SOURCE_DIR}/Source)
include_directories ( ${CMAKE_SOURCE_DIR}/Source/ThirdParty
                              ${CMAKE_SOURCE_DIR}/Source/ThirdParty/rapidjson/include
                              ${CMAKE_SOURCE_DIR}/Source/ThirdParty/kNet/include
                              ${CMAKE_SOURCE_DIR}/Source/ThirdParty/FreeType/include
                              ${CMAKE_SOURCE_DIR}/Source/ThirdParty/Box2D )

# Define source files
file (GLOB SOURCE_FILES *.cpp *.h )

set (SOURCE_FILES ${SOURCE_FILES} ${CSHARP_BINDINGS_SOURCE} ${CSHARPCLOCKWORKPLAYER_BINDINGS_SOURCE}  ${CSHARPCLOCKWORKNET_BINDINGS_SOURCE})

if (MSVC)
    set (EXE_TYPE WIN32)
    set (SOURCE_FILES ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/Build/CMake/Modules/Clockwork.rc)
elseif(APPLE)
    #ICNS
    set(MACOSX_BUNDLE_ICON_FILE Clockwork.icns)
if (NOT IOS)
    set(CLOCKWORK_EDITOR_ICON ${CMAKE_SOURCE_DIR}/Build/CMake/Modules/Clockwork.icns)
endif()
    set_source_files_properties(${CLOCKWORK_EDITOR_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set (EXE_TYPE MACOSX_BUNDLE)
endif()

if (ANDROID)
	include_directories(${CMAKE_SOURCE_DIR}/Source/ThirdParty/SDL/include)
	add_library(ClockworkPlayer SHARED ${SOURCE_FILES} ${CMAKE_SOURCE_DIR}/Source/ThirdParty/SDL/src/main/android/SDL_android_main.c)
else()
	add_executable(ClockworkPlayer ${EXE_TYPE} ${SOURCE_FILES} ${CLOCKWORK_EDITOR_ICON})
endif()

target_link_libraries(ClockworkPlayer ClockworkJS ClockworkPlayerLib ClockworkPlayerJS ${CLOCKWORK_LINK_LIBRARIES})

if (MSVC)
  # pre-Windows 8 can't count on D3DCompiler_47.dll being on system
  add_custom_command (TARGET ClockworkPlayer POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy_if_different \"${D3DCOMPILER_47_DLL}\" \"$<TARGET_FILE_DIR:ClockworkPlayer>/D3DCompiler_47.dll\")

  target_link_libraries(ClockworkPlayer NETCore NETScript)
endif(MSVC)


if (APPLE)

  if (NOT IOS)

    target_link_libraries(ClockworkPlayer NETCore NETScript)

    set (TARGET_PROPERTIES MACOSX_BUNDLE_INFO_PLIST MacOSXBundleInfo.plist.template)

  else()

    set_target_properties(ClockworkPlayer PROPERTIES
      MACOSX_BUNDLE_GUI_IDENTIFIER "com.clockworkgameengine.clockworkplayer"
      XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
      XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
      XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
      XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 9.2
      XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2" # iPhone/iPad
      XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES NO
    )

    set (TARGET_PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Build/CMake/Modules/iOSBundleInfo.plist.template)

  endif()

endif(APPLE)


if (LINUX)

    target_link_libraries(ClockworkPlayer NETCore NETScript)

endif(LINUX)


if (TARGET_PROPERTIES)
    set_target_properties (ClockworkPlayer PROPERTIES ${TARGET_PROPERTIES})
endif ()
