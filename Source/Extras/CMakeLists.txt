# Set project name
project (Clockwork-Extras)

# Find Clockwork library
find_package (Clockwork REQUIRED)
include_directories (${CLOCKWORK_INCLUDE_DIRS})

# All tools must be natively built and output to bin/tool subdir to differentiate them from platform-specific binaries in the bin directory
set_output_directories (${CMAKE_BINARY_DIR}/bin/tool RUNTIME PDB)

# Clockwork extras
add_subdirectory (OgreBatchConverter)
