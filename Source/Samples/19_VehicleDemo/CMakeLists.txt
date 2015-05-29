# Define target name
set (TARGET_NAME 19_VehicleDemo)

# Define source files
define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES})

# Setup target with resource copying
if (EMSCRIPTEN)
    # Override the Clockwork default
    math (EXPR EMSCRIPTEN_TOTAL_MEMORY "64 * 1024 * 1024")
    if (CLOCKWORK_TESTING AND CLOCKWORK_TEST_TIMEOUT LESS 15)
        set (CLOCKWORK_TEST_TIMEOUT 15)
    endif ()
endif ()
setup_main_executable ()

# Setup test cases
setup_test ()
