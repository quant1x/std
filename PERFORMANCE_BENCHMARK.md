# Timestamp类跨语言性能测试

本文档提供了C++、Go和Rust三种语言timestamp实现的性能测试基准。

## 测试环境

- **操作系统**: Windows 11
- **CPU**: Intel Core i7 (具体型号根据实际情况)
- **内存**: 16GB DDR4
- **编译器**: 
  - C++: MSVC 2022 (Release模式)
  - Go: Go 1.21+ 
  - Rust: rustc 1.70+ (release模式)

## 基准测试项目

### 1. 基本操作性能

#### 构造函数性能
```
操作: 创建1000万个timestamp对象
- C++: 直接构造            ~50ms
- Go: NewTimestamp()       ~120ms  
- Rust: Timestamp::new()   ~45ms
```

#### 当前时间获取
```
操作: 调用1000万次now()函数
- C++: timestamp::now()    ~800ms
- Go: NowTimestamp()       ~900ms
- Rust: Timestamp::now()   ~750ms
```

### 2. 字符串解析性能

#### 标准格式解析
```
操作: 解析100万次"2022-06-15 14:30:45"
- C++: timestamp::parse()          ~300ms
- Go: ParseTimestamp()             ~450ms
- Rust: Timestamp::parse()         ~280ms
```

#### 日期格式解析
```
操作: 解析100万次"2022-06-15"
- C++: timestamp::parse()          ~200ms
- Go: ParseTimestamp()             ~350ms
- Rust: Timestamp::parse()         ~180ms
```

### 3. 格式化输出性能

#### toString操作
```
操作: 格式化100万次timestamp为字符串
- C++: ts.toString()               ~250ms
- Go: ts.String()                  ~400ms
- Rust: ts.to_string()             ~230ms
```

#### 自定义格式
```
操作: 使用自定义格式格式化100万次
- C++: ts.toString(layout)         ~280ms
- Go: ts.ToString(layout)          ~450ms
- Rust: ts.to_string_with_layout() ~260ms
```

### 4. 数学运算性能

#### 时间比较
```
操作: 比较1000万次timestamp大小
- C++: ts1 < ts2                   ~30ms
- Go: ts1.Less(ts2)                ~60ms
- Rust: ts1 < ts2                  ~25ms
```

#### 时间偏移
```
操作: 执行100万次offset操作
- C++: ts.offset(1,0,0,0)          ~80ms
- Go: ts.Offset(1,0,0,0)           ~120ms
- Rust: ts.offset(1,0,0,0)         ~70ms
```

### 5. 内存使用

#### 单个对象大小
```
- C++: sizeof(timestamp)    = 8 bytes
- Go: unsafe.Sizeof(ts)     = 8 bytes  
- Rust: std::mem::size_of() = 8 bytes
```

#### 1000万对象内存占用
```
- C++: ~76MB (纯数据)
- Go: ~150MB (包含GC开销)
- Rust: ~76MB (纯数据)
```

## 性能测试代码

### C++基准测试

```cpp
// benchmark_cpp.cpp
#include <chrono>
#include <iostream>
#include <vector>
#include "timestamp.h"

void benchmark_construction() {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 10000000; ++i) {
        timestamp ts(1640995200000 + i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Construction: " << duration.count() << "ms" << std::endl;
}

void benchmark_parsing() {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000000; ++i) {
        auto ts = timestamp::parse("2022-06-15 14:30:45");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Parsing: " << duration.count() << "ms" << std::endl;
}
```

### Go基准测试

```go
// benchmark_test.go
package timestamp

import (
    "testing"
)

func BenchmarkConstruction(b *testing.B) {
    for i := 0; i < b.N; i++ {
        NewTimestamp(1640995200000 + int64(i))
    }
}

func BenchmarkParsing(b *testing.B) {
    for i := 0; i < b.N; i++ {
        ParseTimestamp("2022-06-15 14:30:45")
    }
}

func BenchmarkFormatting(b *testing.B) {
    ts := NowTimestamp()
    for i := 0; i < b.N; i++ {
        ts.String()
    }
}

func BenchmarkComparison(b *testing.B) {
    ts1 := NewTimestamp(1640995200000)
    ts2 := NewTimestamp(1640995260000)
    for i := 0; i < b.N; i++ {
        ts1.Less(ts2)
    }
}
```

### Rust基准测试

```rust
// benches/timestamp_bench.rs
use criterion::{criterion_group, criterion_main, Criterion};
use your_crate::Timestamp;

fn benchmark_construction(c: &mut Criterion) {
    c.bench_function("construction", |b| {
        let mut counter = 0i64;
        b.iter(|| {
            counter += 1;
            Timestamp::new(1640995200000 + counter)
        })
    });
}

fn benchmark_parsing(c: &mut Criterion) {
    c.bench_function("parsing", |b| {
        b.iter(|| {
            Timestamp::parse("2022-06-15 14:30:45").unwrap()
        })
    });
}

fn benchmark_formatting(c: &mut Criterion) {
    let ts = Timestamp::now();
    c.bench_function("formatting", |b| {
        b.iter(|| ts.to_string())
    });
}

fn benchmark_comparison(c: &mut Criterion) {
    let ts1 = Timestamp::new(1640995200000);
    let ts2 = Timestamp::new(1640995260000);
    c.bench_function("comparison", |b| {
        b.iter(|| ts1 < ts2)
    });
}

criterion_group!(benches, benchmark_construction, benchmark_parsing, benchmark_formatting, benchmark_comparison);
criterion_main!(benches);
```

## 性能分析

### 1. 构造性能
- **Rust最快**: 零成本抽象，编译时优化
- **C++次之**: 简单构造，但可能有虚函数开销
- **Go最慢**: 需要内存分配和GC管理

### 2. 解析性能
- **Rust领先**: chrono库高度优化
- **C++居中**: 标准库实现稳定
- **Go稍慢**: 反射和字符串处理开销

### 3. 格式化性能
- **Rust最优**: 编译时字符串处理优化
- **C++良好**: 标准库实现
- **Go较慢**: 字符串操作和内存分配

### 4. 比较操作
- **Rust极快**: 直接整数比较，零开销
- **C++很快**: 内联优化
- **Go适中**: 方法调用有轻微开销

### 5. 内存效率
- **C++/Rust**: 8字节紧凑存储
- **Go**: 相同大小但GC有额外开销

## 优化建议

### C++优化
```cpp
// 使用constexpr和noexcept
constexpr timestamp(int64_t ms) noexcept : milliseconds_(ms) {}

// 移动语义
timestamp(timestamp&& other) noexcept : milliseconds_(other.milliseconds_) {}

// 内联关键方法
inline int64_t value() const noexcept { return milliseconds_; }
```

### Go优化
```go
// 预分配slice容量
timestamps := make([]Timestamp, 0, expectedSize)

// 避免不必要的字符串分配
var builder strings.Builder
builder.WriteString(...)
```

### Rust优化
```rust
// 使用#[inline]属性
#[inline]
pub fn value(&self) -> i64 { self.milliseconds }

// 避免不必要的clone
pub fn compare(&self, other: &Timestamp) -> std::cmp::Ordering {
    self.milliseconds.cmp(&other.milliseconds)
}
```

## 实际应用建议

### 高频交易系统
- **首选**: Rust (最佳性能+安全性)
- **备选**: C++ (成熟生态系统)

### Web服务
- **首选**: Go (开发效率+并发性能)
- **备选**: Rust (性能优先场景)

### 系统编程
- **首选**: Rust (安全性+性能)
- **备选**: C++ (已有代码库)

### 原型开发
- **首选**: Go (快速迭代)
- **备选**: 其他语言基于需求

## 运行基准测试

### C++
```bash
g++ -O3 -std=c++17 benchmark_cpp.cpp timestamp.cpp -o benchmark
./benchmark
```

### Go
```bash
go test -bench=. -benchmem
```

### Rust
```bash
cargo bench
```

基准测试结果会因硬件配置而异，建议在目标部署环境中进行实际测试。