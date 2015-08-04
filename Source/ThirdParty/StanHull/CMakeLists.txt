

# Define target name
set (TARGET_NAME StanHull)

# Define source files
define_source_files ()

# Setup target
setup_library ()

# Install headers for building the Clockwork library
install_header_files (DIRECTORY ./ DESTINATION ${DEST_INCLUDE_DIR}/ThirdParty/StanHull FILES_MATCHING PATTERN *.h BUILD_TREE_ONLY)  # Note: the trailing slash is significant
