@echo off
setlocal enabledelayedexpansion

echo === Quant1x C++ Timestamp Library Build Script ===
echo.

:: Check if cmake is available
cmake --version >nul 2>&1
if errorlevel 1 (
    echo Error: CMake not found in PATH
    echo Please install CMake and add it to your PATH
    pause
    exit /b 1
)

:: Set build directory
set BUILD_DIR=build

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" (
    echo Creating build directory...
    mkdir "%BUILD_DIR%"
)

:: Change to build directory
cd "%BUILD_DIR%"

:: Configure project
echo Configuring project...
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON
if errorlevel 1 (
    echo Error: CMake configuration failed
    pause
    exit /b 1
)

:: Build project
echo.
echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo Error: Build failed
    pause
    exit /b 1
)

:: Run tests if they were built
if exist "tests\Release\timestamp_tests.exe" (
    echo.
    echo Running tests...
    tests\Release\timestamp_tests.exe
) else if exist "tests\timestamp_tests.exe" (
    echo.
    echo Running tests...
    tests\timestamp_tests.exe
) else (
    echo Tests executable not found, skipping tests
)

:: Run example if it was built
if exist "examples\Release\cpp_demo.exe" (
    echo.
    echo Running C++ demo...
    examples\Release\cpp_demo.exe
) else if exist "examples\cpp_demo.exe" (
    echo.
    echo Running C++ demo...
    examples\cpp_demo.exe
) else (
    echo Example executable not found, skipping demo
)

echo.
echo Build completed successfully!
echo.
echo Generated files:
echo - Static library: src\Release\quant1x_timestamp.lib (or src\libquant1x_timestamp.a)
echo - Test executable: tests\Release\timestamp_tests.exe
echo - Example executable: examples\Release\cpp_demo.exe
echo.

pause