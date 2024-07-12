# Add the Sema source files
file(GLOB_RECURSE SEMA_SOURCES "*.cpp")

# Include directories for the Sema
include_directories(${CMAKE_SOURCE_DIR}/include/Sema)

# Create a static library for the Sema
if (SEMA_SOURCES)
    add_library(Sema STATIC ${SEMA_SOURCES})
else()
    message(WARNING "No source files found for the Sema library.")
endif()
