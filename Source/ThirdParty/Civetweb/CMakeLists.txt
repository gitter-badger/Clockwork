

# Define target name
set (TARGET_NAME Civetweb)

# Define source files
define_source_files (GLOB_CPP_PATTERNS src/*.c GLOB_H_PATTERNS include/*.h)

# Define include directory
set (INCLUDE_DIRS include)

# Setup target
setup_library ()

# Install headers for building the Clockwork library
install_header_files (DIRECTORY include/ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/Civetweb FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant
