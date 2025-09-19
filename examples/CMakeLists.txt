# Examples CMakeLists.txt

# Create C++ demo executable
add_executable(cpp_demo
    cpp_demo.cpp
)

# Link against the timestamp library
target_link_libraries(cpp_demo PRIVATE quant1x::timestamp)

# Set target properties
set_target_properties(cpp_demo PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Compiler-specific options
if(MSVC)
    target_compile_options(cpp_demo PRIVATE /W4)
else()
    target_compile_options(cpp_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Optional: Install examples
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    install(TARGETS cpp_demo
        RUNTIME DESTINATION bin/examples
    )
endif()