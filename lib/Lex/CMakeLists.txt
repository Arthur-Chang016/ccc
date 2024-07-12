# Add the Lex source files
file(GLOB_RECURSE LEX_SOURCES "*.cpp")

# Include directories for the Lex
include_directories(${CMAKE_SOURCE_DIR}/include/Lex)

# Create a static library for the Lex
if (LEX_SOURCES)
    add_library(Lex STATIC ${LEX_SOURCES})
else()
    message(WARNING "No source files found for the Lex library.")
endif()
