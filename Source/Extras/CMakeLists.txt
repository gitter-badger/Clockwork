# Set project name
project (Clockwork-Extras)

# Find Clockwork library
find_package (Clockwork REQUIRED)
include_directories (${CLOCKWORK_INCLUDE_DIRS})

# All tools must be natively built and output to bin/tool subdir to differentiate them from target platfrom binaries in the bin directory
set_tool_output_directories ()

# Clockwork extras
add_subdirectory (OgreBatchConverter)
