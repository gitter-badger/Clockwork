

# Define target name
set (TARGET_NAME Detour)

# Define source files
define_source_files (GLOB_CPP_PATTERNS source/*.cpp GLOB_H_PATTERNS include/*.h)

# Define dependency libs
set (INCLUDE_DIRS include)

# Setup target
setup_library ()

# Install headers for building the Clockwork library
install_header_files (DIRECTORY include/ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/Detour FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant
