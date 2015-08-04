

# Define target name
set (TARGET_NAME ClockworkPlayer)

# Define source files
define_source_files ()

# Setup target with resource copying
setup_main_executable (NOBUNDLE)

# Setup test cases
if (CLOCKWORK_ANGELSCRIPT)
    setup_test (NAME Editor OPTIONS Scripts/Editor.as -w)
    setup_test (NAME NinjaSnowWar OPTIONS Scripts/NinjaSnowWar.as -w)
    setup_test (NAME SpritesAS OPTIONS Scripts/03_Sprites.as -w)
endif ()
if (CLOCKWORK_LUA)
    setup_test (NAME SpritesLua OPTIONS LuaScripts/03_Sprites.lua -w)
endif ()

# Symlink/copy helper shell scripts or batch files to invoke ClockworkPlayer
if (NOT IOS AND NOT ANDROID AND NOT EMSCRIPTEN AND NOT CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    # Ensure the output directory exist before creating the symlinks
    file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    foreach (FILE Editor NinjaSnowWar)
        create_symlink (${CMAKE_SOURCE_DIR}/bin/${FILE}${SCRIPT_EXT} ${CMAKE_BINARY_DIR}/bin/${FILE}${SCRIPT_EXT} FALLBACK_TO_COPY)
    endforeach ()
endif ()
