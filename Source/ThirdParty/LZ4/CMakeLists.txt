# Define target name
set (TARGET_NAME LZ4)

# Define source files
define_source_files (GLOB_CPP_PATTERNS *.c)

# Setup target
setup_library ()

# Install headers for building the Clockwork library
install_header_files (DIRECTORY ./ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/LZ4 FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant
