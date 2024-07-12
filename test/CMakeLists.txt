# Enable testing
enable_testing()

# Add the test executable
file(GLOB_RECURSE TEST_SOURCES "*.cpp")

if (TEST_SOURCES)
    add_executable(runTests ${TEST_SOURCES})
    # Link the test executable with necessary libraries
    target_link_libraries(runTests Lex Parse Sema CodeGen)
else()
    message(WARNING "No source files found for the runTests library.")
endif()

# Add tests
add_test(NAME LexerTests COMMAND runTests)
