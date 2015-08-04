

# Define target name
set (TARGET_NAME RampGenerator)

# Define source files
define_source_files ()

# Define dependency libs
set (LIBS STB)

# Setup target
if (APPLE)
    setup_macosx_linker_flags (CMAKE_EXE_LINKER_FLAGS)
endif ()
setup_executable ()
