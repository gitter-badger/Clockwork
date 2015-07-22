clockwork

# Define target name
set (TARGET_NAME 20_HugeObjectCount)

# Define source files
define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES})

# Setup target with resource copying
if (EMSCRIPTEN)
    math (EXPR EMSCRIPTEN_TOTAL_MEMORY "128 * 1024 * 1024")     # Override the Clockwork default
endif ()
setup_main_executable ()

# Setup test cases
setup_test ()
