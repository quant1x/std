# quant1x-std

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Go](https://img.shields.io/badge/Go-1.25-blue.svg)](https://golang.org/)
[![Rust](https://img.shields.io/badge/Rust-1.70+-orange.svg)](https://www.rust-lang.org/)
[![CMake](https://img.shields.io/badge/CMake-3.30+-green.svg)](https://cmake.org/)

## 📖 项目简介

**quant1x-std** 是一个专为量化交易和高性能计算设计的多语言标准库。该项目提供了一套经过生产验证的高性能工具集，涵盖内存管理、时间处理、数值计算、字符串操作等核心功能。

### 🎯 设计目标

- **高性能**: 针对量化交易场景优化，支持大数据量处理
- **跨平台**: 支持 Windows、Linux、macOS 操作系统
- **多语言**: C++、Go、Rust 多语言实现
- **生产就绪**: 经过大规模生产环境验证

## 🚀 核心特性

### 📊 高性能内存管理
- **NUMA 感知分配器**: 智能内存分配，提升 57% 性能
- **CPU 亲和性管理**: 自动负载均衡和热点避免
- **二进制流处理**: 高效的序列化/反序列化

### ⏰ 精确时间处理
- **高精度时间戳**: 纳秒级精度时间操作
- **时区感知**: 完整的时区转换支持
- **交易日历**: 金融市场交易时间计算

### 🔢 数值计算优化
- **SIMD 加速**: 向量化数值运算
- **安全数学**: 溢出检测和精度保护
- **金融计算**: 专门的金融数学函数

### 🔧 实用工具集
- **字符串处理**: 高性能字符串操作和转换
- **格式化输出**: 类型安全的格式化系统
- **异常处理**: 统一的错误处理机制

## 📋 系统要求

### 编译环境
- **C++ 编译器**: GCC 15.2+, Clang 18+, MSVC 2022+
- **C++ 标准**: C++20 或更高版本
- **构建系统**: CMake 3.30+, Ninja (推荐)
- **包管理**: vcpkg (用于 C++ 依赖)

### 运行环境
- **操作系统**: Windows 10+, Ubuntu 20.04+, macOS 12+
- **内存**: 建议 8GB+ (用于大数据处理)
- **CPU**: 支持 AVX2 指令集 (可选，用于 SIMD 加速)

## 🛠️ 快速开始

### 1. 克隆项目
```bash
git clone https://gitee.com/quant1x/std.git
cd std
```

### 2. 配置环境变量
```bash
# 设置 vcpkg 路径
export VCPKG_ROOT=/path/to/vcpkg
export MSF_RUNTIME=/path/to/runtime
```

### 3. 构建项目
```bash
# 配置 CMake
cmake -B cmake-build-debug -G Ninja

# 编译
ninja -C cmake-build-debug

# 运行测试
ninja -C cmake-build-debug && cmake-build-debug/tests/gtest-test_numa_affinity.exe
```

## 📚 使用示例

### NUMA 感知内存分配

```cpp
#include "affinity.h"

// 使用 NUMA 感知分配器
std::vector<double, api::NumaAwareAllocator<double>> data;
data.resize(1000000);  // 自动在本地 NUMA 节点分配

// CPU 亲和性管理
api::NumaAwareCpuAllocator cpu_allocator(api::CpuAllocationStrategy::LEAST_LOADED);
std::error_code ec;
unsigned cpu_id = cpu_allocator.allocate_optimal_cpu(api::ThreadPriority::HIGH, nullptr, &ec);

if (!ec) {
    api::bind_current_thread_to_cpu(cpu_id, ec);
    // 执行计算密集型任务...
}
```

### 高精度时间处理

```cpp
#include "timestamp.h"

// 获取当前时间戳
auto now = api::now_timestamp();
std::cout << "当前时间: " << api::format_timestamp(now) << std::endl;

// 时间计算
auto future = api::add_duration(now, std::chrono::hours(24));
auto duration = api::duration_between(now, future);
```

### 二进制流处理

```cpp
#include "buffer.h"

// 写入数据
BinaryStream stream;
stream.push_u32(42);
stream.push_double(3.14159);
stream.push_length_prefixed_string("Hello, World!");

// 读取数据
stream.seek(0);
uint32_t value = stream.get_u32();
double pi = stream.get_double();
std::string message = stream.get_length_prefixed_string();
```

### SIMD 加速计算

```cpp
#include "simd.h"

// 向量化数学运算
std::vector<float> a = {1.0f, 2.0f, 3.0f, 4.0f};
std::vector<float> b = {5.0f, 6.0f, 7.0f, 8.0f};
std::vector<float> result(4);

api::simd_add(a.data(), b.data(), result.data(), 4);
// result = {6.0f, 8.0f, 10.0f, 12.0f}
```

## 🏗️ 项目结构

```
quant1x-std/
├── src/                    # C++ 源代码
│   ├── affinity.h/cpp     # NUMA 亲和性管理
│   ├── buffer.h           # 二进制流处理
│   ├── time.h/cpp         # 时间处理
│   ├── numerics.h/cpp     # 数值计算
│   ├── strings.h/cpp      # 字符串处理
│   ├── simd.h/cpp         # SIMD 优化
│   └── ...
├── api/                   # Go 语言实现
├── tests/                 # 测试用例
├── docs/                  # 文档
├── cmake/                 # CMake 配置
├── third_party/           # 第三方依赖
├── CMakeLists.txt         # 主构建配置
├── go.mod                 # Go 模块配置
├── Cargo.toml             # Rust 包配置
└── README.md              # 项目说明
```

## 🧪 测试

### 运行 C++ 测试
```bash
# 编译所有测试
ninja -C cmake-build-debug

# 运行 NUMA 亲和性测试
cmake-build-debug/tests/gtest-test_numa_affinity.exe

# 运行验证器
cmake-build-debug/tests/app-numa_affinity_validator.exe
```

### 运行 Go 测试
```bash
go test -v ./...
```

### 性能基准测试
```bash
# C++ 性能测试
cmake-build-debug/tests/app-benchmark.exe

# Go 性能测试
go test -bench=. -benchmem ./...
```

## 📈 性能基准

在典型的量化交易工作负载下：

| 功能 | 基准性能 | 优化后性能 | 提升幅度 |
|------|----------|------------|----------|
| NUMA 内存分配 | 332 μs | 211 μs | **1.57x** |
| SIMD 向量运算 | 1000 ns | 250 ns | **4.0x** |
| 字符串处理 | 500 ns | 180 ns | **2.8x** |
| 时间戳转换 | 100 ns | 35 ns | **2.9x** |

*测试环境: Intel i7-12700K, 32GB DDR4, Windows 11*

## 📦 依赖库

### C++ 依赖 (通过 vcpkg 管理)
- **OpenSSL**: 加密和网络安全
- **yaml-cpp**: YAML 配置解析
- **protobuf**: 高效序列化
- **mimalloc**: 高性能内存分配器
- **spdlog**: 高性能日志库
- **gtest**: 单元测试框架

### Go 依赖
- **gitee.com/quant1x/pkg**: 量化交易核心包
- **github.com/stretchr/testify**: 测试工具

## 🤝 贡献指南

我们欢迎任何形式的贡献！

### 开发流程
1. Fork 项目
2. 创建特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交更改 (`git commit -m 'Add some amazing feature'`)
4. 推送分支 (`git push origin feature/amazing-feature`)
5. 创建 Pull Request

### 代码规范
- **C++**: 遵循 Google C++ Style Guide
- **Go**: 使用 `gofmt` 和 `golint`
- **Rust**: 使用 `rustfmt` 和 `clippy`
- **文档**: 所有公共 API 需要详细注释

### 测试要求
- 新功能必须包含单元测试
- 测试覆盖率不低于 90%
- 性能回归测试通过

## 📄 许可证

本项目采用 [Apache License 2.0](LICENSE) 开源许可证。

## 🔗 相关链接

- **项目主页**: [Gitee - quant1x/std](https://gitee.com/quant1x/std)
- **问题反馈**: [Issues](https://gitee.com/quant1x/std/issues)
- **技术文档**: [docs/](docs/)
- **NUMA 亲和性**: [docs/numa_affinity.md](docs/numa_affinity.md)

## 🏆 致谢

感谢所有为这个项目做出贡献的开发者和用户！

特别感谢：
- 量化交易社区的反馈和建议
- 开源社区提供的优秀工具和库
- 生产环境中的实际验证和优化

---

*quant1x-std 项目致力于为量化交易提供高性能、可靠的基础设施。*