#!/bin/bash
# 安装脚本测试

echo "构建和安装 quant1x-std..."

# 配置
cmake -B build -G Ninja

# 编译
ninja -C build

# 安装
ninja -C build install

echo "安装完成！"
echo "下游项目现在可以使用："
echo "find_package(quant1x-std REQUIRED)"
echo "target_link_libraries(target PRIVATE quant1x::std)"