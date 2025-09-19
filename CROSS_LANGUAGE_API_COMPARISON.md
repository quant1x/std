# Timestamp类的跨语言实现对比

本文档展示了C++、Go和Rust三种语言中timestamp类的API对比，实现了完全一致的功能。

## 设计目标

- **API一致性**: 三种语言提供相同的方法和功能
- **行为一致性**: 相同输入产生相同输出
- **性能优化**: 每种语言都充分利用其特性
- **类型安全**: 利用各语言的类型系统防止错误

## 核心常量对比

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 毫秒/秒 | `milliseconds_per_second` | `MillisecondsPerSecond` | `MILLISECONDS_PER_SECOND` |
| 毫秒/分 | `milliseconds_per_minute` | `MillisecondsPerMinute` | `MILLISECONDS_PER_MINUTE` |
| 毫秒/小时 | `milliseconds_per_hour` | `MillisecondsPerHour` | `MILLISECONDS_PER_HOUR` |
| 毫秒/天 | `milliseconds_per_day` | `MillisecondsPerDay` | `MILLISECONDS_PER_DAY` |

## 构造函数对比

### 基本构造

**C++:**
```cpp
timestamp ts(1640995200000);              // 从毫秒数
timestamp ts(system_clock::now());        // 从时间点
timestamp ts("2022-01-01 15:30:45");      // 从字符串
timestamp ts(2022, 1, 1, 15, 30, 45, 123); // 从日期时间
```

**Go:**
```go
ts := NewTimestamp(1640995200000)                    // 从毫秒数
ts := NewTimestampFromTime(time.Now())               // 从time.Time
ts, _ := NewTimestampFromString("2022-01-01 15:30:45") // 从字符串
ts := NewTimestampFromDate(2022, 1, 1, 15, 30, 45, 123) // 从日期时间
```

**Rust:**
```rust
let ts = Timestamp::new(1640995200000);              // 从毫秒数
let ts = Timestamp::from_datetime(Local::now());     // 从DateTime
let ts = Timestamp::parse("2022-01-01 15:30:45")?;  // 从字符串
let ts = Timestamp::from_date(2022, 1, 1, 15, 30, 45, 123)?; // 从日期时间
```

### 工厂方法

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 当前时间 | `timestamp::now()` | `NowTimestamp()` | `Timestamp::now()` |
| 零值 | `timestamp::zero()` | `ZeroTimestamp()` | `Timestamp::zero()` |
| 盘前时间 | `timestamp::pre_market_time(y,m,d)` | `PreMarketTimestamp(y,m,d)` | `Timestamp::pre_market_time(y,m,d)` |

## 核心方法对比

### 获取值

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 毫秒值 | `ts.value()` | `ts.Value()` | `ts.value()` |
| 是否为空 | `ts.empty()` | `ts.IsEmpty()` | `ts.is_empty()` |
| 提取年月日 | `ts.extract()` | `ts.Extract()` | `ts.extract()` |

### 时间操作

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 当天零点 | `ts.start_of_day()` | `ts.StartOfDay()` | `ts.start_of_day()` |
| 今天N点 | `ts.today(h,m,s,ms)` | `ts.Today(h,m,s,ms)` | `ts.today(h,m,s,ms)` |
| 时间偏移 | `ts.offset(h,m,s,ms)` | `ts.Offset(h,m,s,ms)` | `ts.offset(h,m,s,ms)` |
| Floor操作 | `ts.floor()` | `ts.Floor()` | `ts.floor()` |
| Ceil操作 | `ts.ceil()` | `ts.Ceil()` | `ts.ceil()` |

### 格式化

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 默认字符串 | `ts.toString()` | `ts.String()` | `ts.to_string()` |
| 自定义格式 | `ts.toString(layout)` | `ts.ToString(layout)` | `ts.to_string_with_layout(layout)` |
| 仅日期 | `ts.only_date()` | `ts.OnlyDate()` | `ts.only_date()` |
| 仅时间 | `ts.only_time()` | `ts.OnlyTime()` | `ts.only_time()` |
| YYYYMMDD | `ts.yyyymmdd()` | `ts.YYYYMMDD()` | `ts.yyyymmdd()` |

### 比较操作

| 功能 | C++ | Go | Rust |
|------|-----|-------|------|
| 等于 | `ts1 == ts2` | `ts1.Equal(ts2)` | `ts1 == ts2` |
| 小于 | `ts1 < ts2` | `ts1.Less(ts2)` | `ts1 < ts2` |
| 大于 | `ts1 > ts2` | `ts1.Greater(ts2)` | `ts1 > ts2` |
| 同一天 | `ts1.is_same_date(ts2)` | `ts1.IsSameDate(ts2)` | `ts1.is_same_date(&ts2)` |

## 解析功能对比

### 日期时间解析

**C++:**
```cpp
auto ts = timestamp::parse("2022-06-15 14:30:45");
auto ts = timestamp::parse_time("14:30:45");
```

**Go:**
```go
ts, err := ParseTimestamp("2022-06-15 14:30:45")
ts, err := ParseTimeOnly("14:30:45")
```

**Rust:**
```rust
let ts = Timestamp::parse("2022-06-15 14:30:45")?;
let ts = Timestamp::parse_time("14:30:45")?;
```

### 支持的格式

所有三种实现都支持以下格式：
- `"2022-01-01 15:30:45"`
- `"2022-01-01"`
- `"2022/01/01 15:30:45"`
- `"2022/01/01"`
- `"15:30:45"` (仅时间)

## 类型转换对比

### 与原生类型互转

**C++:**
```cpp
int64_t ms = ts;                    // 隐式转换
timestamp ts = 1640995200000;       // 隐式转换
```

**Go:**
```go
ms := ts.AsTimestamp()              // 转为int64
ts := FromTimestamp(ms)             // 从int64创建
```

**Rust:**
```rust
let ms: i64 = ts.into();            // 转为i64
let ts = Timestamp::from(ms);       // 从i64创建
```

### 与标准库时间类型互转

**C++:**
```cpp
auto tp = chrono::system_clock::now();
timestamp ts(tp);
```

**Go:**
```go
ts := NewTimestampFromTime(time.Now())
t := ts.ToTime()
```

**Rust:**
```rust
let ts = Timestamp::from_datetime(Local::now());
let dt = ts.to_datetime();
```

## 错误处理对比

| 语言 | 错误处理方式 | 示例 |
|------|--------------|------|
| C++ | 异常 | `try { auto ts = timestamp::parse(str); } catch(...) {}` |
| Go | 返回错误 | `ts, err := ParseTimestamp(str); if err != nil {}` |
| Rust | Result类型 | `let ts = Timestamp::parse(str)?;` |

## 性能特点

### C++
- 零开销抽象
- 编译时优化
- 内存布局紧凑

### Go
- 简洁的API设计
- 垃圾回收管理内存
- 优秀的并发性能

### Rust
- 零成本抽象
- 编译时内存安全
- 无运行时开销

## 使用建议

### 选择C++当你需要：
- 最高性能
- 与现有C++代码集成
- 精确的内存控制

### 选择Go当你需要：
- 快速开发
- 简单的部署
- 优秀的并发处理

### 选择Rust当你需要：
- 内存安全保证
- 零成本抽象
- 系统级编程

## 示例：相同功能的实现

创建时间戳，格式化输出，比较操作：

**C++:**
```cpp
auto ts1 = timestamp::now();
auto ts2 = timestamp(2022, 6, 15, 14, 30, 45, 0);
std::cout << ts2.toString() << std::endl;
if (ts1 > ts2) {
    std::cout << "ts1 is later" << std::endl;
}
```

**Go:**
```go
ts1 := NowTimestamp()
ts2 := NewTimestampFromDate(2022, 6, 15, 14, 30, 45, 0)
fmt.Println(ts2.String())
if ts1.Greater(ts2) {
    fmt.Println("ts1 is later")
}
```

**Rust:**
```rust
let ts1 = Timestamp::now();
let ts2 = Timestamp::from_date(2022, 6, 15, 14, 30, 45, 0).unwrap();
println!("{}", ts2);
if ts1 > ts2 {
    println!("ts1 is later");
}
```

所有三个版本都会产生相同的输出结果，体现了跨语言API的一致性设计。