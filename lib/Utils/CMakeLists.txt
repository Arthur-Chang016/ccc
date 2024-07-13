# Add the Utils source files
file(GLOB_RECURSE UTILS_SOURCES "*.cpp")

# Include directories for the Utils
include_directories(${CMAKE_SOURCE_DIR}/include/Utils)

# Create a static library for the Utils
if (UTILS_SOURCES)
    add_library(Utils STATIC ${UTILS_SOURCES})
else()
    message(WARNING "No source files found for the Utils library.")
endif()
