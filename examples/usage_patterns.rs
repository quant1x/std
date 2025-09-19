// 演示不同的导入和使用方式

fn main() {
    println!("=== 扁平化命名空间演示 ===\n");
    
    // 方式1: 直接导入 (推荐，简洁明了)
    println!("1. 直接导入:");
    {
        use quant1x_std::Timestamp;
        let ts = Timestamp::now();
        println!("   use quant1x_std::Timestamp;");
        println!("   Timestamp::now() -> {}", ts.to_string());
    }
    
    // 方式2: 带别名导入
    println!("\n2. 别名导入:");
    {
        use quant1x_std::Timestamp as Ts;
        let ts = Ts::now();
        println!("   use quant1x_std::Timestamp as Ts;");
        println!("   Ts::now() -> {}", ts.to_string());
    }
    
    // 方式3: 完整路径调用
    println!("\n3. 完整路径:");
    {
        let ts = quant1x_std::Timestamp::now();
        println!("   quant1x_std::Timestamp::now();");
        println!("   -> {}", ts.to_string());
    }
    
    println!("\n=== 对比C++用法 ===");
    println!("C++:  quant1x::timestamp ts = quant1x::timestamp::now();");
    println!("Rust: use quant1x_std::Timestamp;");
    println!("      let ts = Timestamp::now();");
    
    println!("\n✓ 扁平化设计更简洁!");
    println!("✓ 推荐使用 use quant1x_std::Timestamp;");
}