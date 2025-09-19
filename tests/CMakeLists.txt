#include(Catch)

# 定义一个宏，用于创建 一般性的测试 可执行文件
macro(add_app_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "app-"
    set(target_name "app-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        quant1x-std
    )
endmacro()

# 定义一个宏，用于创建 Catch2单元测试 可执行文件
macro(add_catch2_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "catch2-"
    set(target_name "catch2-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    #catch_discover_tests(${target_name})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        quant1x-std test-catch2 test-benchmark
    )
endmacro()

# 定义一个宏，用于创建 GTest单元测试 可执行文件
macro(add_gtest_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "gtest-"
    set(target_name "gtest-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        quant1x-std test-gtest
    )
endmacro()

# 定义一个宏，用于创建 benchmark基准测试 可执行文件
macro(add_benchmark_executable source_file)
    # 获取源文件名（不含扩展名）
    get_filename_component(test_name ${source_file} NAME_WE)

    # 添加前缀 "benchmark-"
    set(target_name "benchmark-${test_name}")

    # 创建带前缀的可执行文件
    add_executable(${target_name} ${source_file})

    # 添加头文件路径
    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/include
    )

    # 链接依赖库（例如你提到的 utils）
    target_link_libraries(${target_name} PRIVATE
        quant1x-std test-benchmark
    )
endmacro()

add_gtest_executable(test_timestamp.cpp)