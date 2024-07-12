# Add the Parse source files
file(GLOB_RECURSE PARSE_SOURCES "*.cpp")

# Include directories for the Parse
include_directories(${CMAKE_SOURCE_DIR}/include/Parse)

# Create a static library for the Parse
if (PARSE_SOURCES)
    add_library(Parse STATIC ${PARSE_SOURCES})
else()
    message(WARNING "No source files found for the Parse library.")
endif()
