use quant1x_std::Timestamp;

fn main() {
    println!("=== Quant1X 命名空间演示 ===");
    println!("Crate名称: quant1x_std");
    println!("命名空间: quant1x_std::");
    println!();
    
    // 使用直接的命名空间
    let ts1 = Timestamp::now();
    println!("使用 quant1x_std::Timestamp::now(): {}", ts1.to_string());
    
    let ts2 = Timestamp::parse("2022-06-15 14:30:45").unwrap();
    println!("使用 quant1x_std::Timestamp::parse(): {}", ts2.to_string());
    
    let ts3 = Timestamp::parse_time("14:30:45").unwrap();
    println!("使用 quant1x_std::Timestamp::parse_time(): {}", ts3.to_string());
    
    println!();
    println!("✓ 命名空间设计成功！");
    println!("✓ 用户导入: use quant1x_std::Timestamp;");
    println!("✓ 使用方式: Timestamp::now()");
    println!("✓ 与C++命名空间保持一致");
}