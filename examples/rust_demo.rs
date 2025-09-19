// Rust implementation demo
use quant1x_std::Timestamp;

fn main() {
    println!("=== Rust版本的timestamp库演示 ===\n");

    // 1. 创建时间戳的各种方式
    println!("1. 创建时间戳:");

    // 从毫秒数创建
    let ts1 = Timestamp::new(1640995200000);
    println!("从毫秒数创建: {} (值: {})", ts1, ts1.value());

    // 从当前时间创建
    let ts2 = Timestamp::now();
    println!("当前时间戳: {}", ts2);

    // 从日期时间创建
    let ts3 = Timestamp::from_date(2022, 1, 1, 15, 30, 45, 123).unwrap();
    println!("从日期创建: {}", ts3);

    // 从字符串解析
    match Timestamp::parse("2022-06-15 10:30:00") {
        Ok(ts4) => println!("从字符串解析: {}", ts4),
        Err(e) => println!("解析错误: {}", e),
    }

    // 零值时间戳
    let zero = Timestamp::zero();
    println!("零值时间戳: {} (是否为空: {})", zero, zero.is_empty());

    println!("\n2. 时间操作:");

    // 当天零点
    let start_of_day = ts3.start_of_day();
    println!("当天零点: {}", start_of_day);

    // 当天指定时间
    if let Some(today_9am) = ts3.today(9, 0, 0, 0) {
        println!("当天9点: {}", today_9am);
    }

    // 时间偏移
    let offset = ts3.offset(2, 30, 0, 0); // 偏移2.5小时
    println!("偏移2.5小时后: {}", offset);

    // 盘前时间
    if let Some(pre_market) = ts3.pre_market_time_from_current() {
        println!("盘前时间: {}", pre_market);
    }

    // Floor和Ceil操作
    let floor = ts3.floor();
    let ceil = ts3.ceil();
    println!("Floor操作: {}", floor);
    println!("Ceil操作: {}", ceil);

    println!("\n3. 格式化操作:");

    // 各种格式化
    println!("完整时间: {}", ts3.to_string_with_layout("%Y-%m-%d %H:%M:%S%.3f"));
    println!("仅日期: {}", ts3.only_date());
    println!("仅时间: {}", ts3.only_time());
    println!("缓存日期: {}", ts3.cache_date());
    println!("YYYYMMDD: {}", ts3.yyyymmdd());

    // 提取年月日
    let (year, month, day) = ts3.extract();
    println!("提取日期: {}年{}月{}日", year, month, day);

    println!("\n4. 比较操作:");

    let ts5 = Timestamp::from_date(2022, 1, 2, 10, 0, 0, 0).unwrap();

    println!("ts3 < ts5: {}", ts3 < ts5);
    println!("ts3 > ts5: {}", ts3 > ts5);
    println!("ts3 == ts3: {}", ts3 == ts3);
    println!("ts3 != ts5: {}", ts3 != ts5);
    println!("ts3 <= ts5: {}", ts3 <= ts5);
    println!("ts3 >= ts5: {}", ts3 >= ts5);

    println!("\n5. 特殊功能:");

    // 检查是否同一天
    let same_day1 = Timestamp::from_date(2022, 1, 1, 8, 0, 0, 0).unwrap();
    let same_day2 = Timestamp::from_date(2022, 1, 1, 20, 0, 0, 0).unwrap();
    let diff_day = Timestamp::from_date(2022, 1, 2, 8, 0, 0, 0).unwrap();

    println!("同一天检查 (1月1日8点 vs 1月1日20点): {}", same_day1.is_same_date(&same_day2));
    println!("不同天检查 (1月1日8点 vs 1月2日8点): {}", same_day1.is_same_date(&diff_day));

    // 盘前时间创建
    if let Some(pre_market_timestamp) = Timestamp::pre_market_time(2022, 1, 1) {
        println!("盘前时间戳: {}", pre_market_timestamp);
    }

    println!("\n6. 时间解析:");

    // 解析各种格式
    let formats = vec![
        "2022-12-25 14:30:00",
        "2022-12-25",
        "2022/12/25 14:30:00",
    ];

    for format in &formats {
        match Timestamp::parse(format) {
            Ok(parsed) => println!("解析 '{}': {}", format, parsed),
            Err(e) => println!("解析 '{}' 失败: {}", format, e),
        }
    }

    // 仅解析时间
    match Timestamp::parse_time("14:30:45") {
        Ok(time_only) => println!("仅解析时间 '14:30:45': {}", time_only),
        Err(e) => println!("解析时间失败: {}", e),
    }

    println!("\n7. 类型转换:");

    // 转换为Unix毫秒时间戳
    let unix_milli = ts3.unix_millis();
    println!("Unix毫秒时间戳: {}", unix_milli);

    // 转换为chrono::DateTime
    let dt = ts3.to_datetime();
    println!("转换为chrono::DateTime: {}", dt.format("%Y-%m-%d %H:%M:%S"));

    println!("\n=== 演示完成 ===");
    println!("Rust版本特点:");
    println!("- 类型安全和内存安全");
    println!("- 零成本抽象");
    println!("- 丰富的错误处理");
    println!("- 与chrono库无缝集成");
    println!("- 保持与C++/Go版本的API一致性");
}