# Define target name
set (TARGET_NAME AssetImporter)

# Define source files
define_source_files ()

# Define dependency libs
add_subdirectory (../../ThirdParty/Assimp ../../ThirdParty/Assimp)
set (LIBS Assimp)
set (INCLUDE_DIRS ../../ThirdParty/Assimp/include)

# Setup target
if (APPLE)
    setup_macosx_linker_flags (CMAKE_EXE_LINKER_FLAGS)
endif ()
setup_executable ()
