# Tests CMakeLists.txt

# Find or fetch Google Test
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    # Fetch Google Test if not found
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50576ffc7a64d73ee70b5c2a7e3cee.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Create test executable
add_executable(timestamp_tests
    test_timestamp.cpp
)

# Link libraries
target_link_libraries(timestamp_tests PRIVATE
    quant1x::timestamp
    gtest_main
)

# Set target properties
set_target_properties(timestamp_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Compiler-specific options
if(MSVC)
    target_compile_options(timestamp_tests PRIVATE /W4)
else()
    target_compile_options(timestamp_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(timestamp_tests)