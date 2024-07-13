cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ccc VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(ccc ${SOURCES})

# Specify the output directories
set_target_properties(ccc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add subdirectories for components (optional, depending on your structure)
add_subdirectory(lib)
add_subdirectory(test)

# Link the utils library to the ccc executable
target_link_libraries(ccc PRIVATE Utils Lex)
