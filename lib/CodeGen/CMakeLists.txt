# Add the CodeGen source files
file(GLOB_RECURSE CODEGEN_SOURCES "*.cpp")

# Include directories for the CodeGen
include_directories(${CMAKE_SOURCE_DIR}/include/CodeGen)

# Create a static library for the CodeGen
if (CODEGEN_SOURCES)
    add_library(codegen STATIC ${CODEGEN_SOURCES})
else()
    message(WARNING "No source files found for the CodeGen library.")
endif()
