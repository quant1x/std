#!/bin/bash

echo "=== Quant1x C++ Timestamp Library Build Script ==="
echo

# Check if cmake is available
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake not found"
    echo "Please install CMake"
    exit 1
fi

# Set build directory
BUILD_DIR="build"

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
fi

# Change to build directory
cd "$BUILD_DIR"

# Configure project
echo "Configuring project..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
if [ $? -ne 0 ]; then
    echo "Error: CMake configuration failed"
    exit 1
fi

# Build project
echo
echo "Building project..."
cmake --build . --config Release -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
if [ $? -ne 0 ]; then
    echo "Error: Build failed"
    exit 1
fi

# Run tests if they were built
if [ -x "tests/timestamp_tests" ]; then
    echo
    echo "Running tests..."
    ./tests/timestamp_tests
elif [ -x "tests/Release/timestamp_tests" ]; then
    echo
    echo "Running tests..."
    ./tests/Release/timestamp_tests
else
    echo "Tests executable not found, skipping tests"
fi

# Run example if it was built
if [ -x "examples/cpp_demo" ]; then
    echo
    echo "Running C++ demo..."
    ./examples/cpp_demo
elif [ -x "examples/Release/cpp_demo" ]; then
    echo
    echo "Running C++ demo..."
    ./examples/Release/cpp_demo
else
    echo "Example executable not found, skipping demo"
fi

echo
echo "Build completed successfully!"
echo
echo "Generated files:"
echo "- Static library: src/libquant1x_timestamp.a"
echo "- Test executable: tests/timestamp_tests"
echo "- Example executable: examples/cpp_demo"
echo