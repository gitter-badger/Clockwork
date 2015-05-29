# Define target name
set (TARGET_NAME 21_AngelScriptIntegration)

# Define source files
define_source_files (EXTRA_H_FILES ${COMMON_SAMPLE_H_FILES})

# Setup target with resource copying
setup_main_executable ()

# Setup test cases
setup_test ()
