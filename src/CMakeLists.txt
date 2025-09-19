# Source library CMakeLists.txt

# Create timestamp library
add_library(quant1x_timestamp STATIC
    timestamp.cpp
    timestamp.h
)

# Set target properties
set_target_properties(quant1x_timestamp PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Include directories for this target
target_include_directories(quant1x_timestamp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/quant1x>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler-specific optimizations
if(MSVC)
    target_compile_options(quant1x_timestamp PRIVATE
        /W4 /WX /permissive-
    )
else()
    target_compile_options(quant1x_timestamp PRIVATE
        -Wall -Wextra -Wpedantic -Werror
        -fPIC  # Position independent code for shared library
    )
endif()

# Link libraries (if needed in the future)
# target_link_libraries(quant1x_timestamp PRIVATE some_library)

# Export target for use by other projects
target_compile_features(quant1x_timestamp PUBLIC cxx_std_17)

# Alias for internal use
add_library(quant1x::timestamp ALIAS quant1x_timestamp)