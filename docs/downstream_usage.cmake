# 下游项目的 CMakeLists.txt 使用示例

cmake_minimum_required(VERSION 3.20)
project(my_project)

# 查找 quant1x 包
find_package(quant1x-std REQUIRED)

# 创建你的可执行文件
add_executable(my_app main.cpp)

# 链接 quant1x::std 库
target_link_libraries(my_app PRIVATE quant1x::std)

# 现在可以在代码中使用:
# #include <quant1x/timestamp.h>
# #include <quant1x/time.h>
# 
# 使用 quant1x::timestamp 类
# 使用 api::parse_date() 等函数