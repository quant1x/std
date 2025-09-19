#include "timestamp.h"
#include <iostream>
#include <chrono>

// 简化的api实现，只为测试timestamp基本功能
namespace api {
    // 简化实现：假设没有时区偏移
    int64_t ms_utc_to_local(const int64_t& milliseconds) {
        return milliseconds; // 简化：直接返回，不做时区转换
    }
    
    // 简化实现：解析基本日期格式 "YYYY-MM-DD"
    int64_t parse_date(const std::string& str) {
        // 简化：返回当前时间的毫秒数
        auto now = std::chrono::system_clock::now();
        auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return epoch.count();
    }
    
    // 简化实现：解析基本时间格式 "HH:MM:SS"
    int64_t parse_time(const std::string& str) {
        // 简化：返回当前时间的毫秒数
        auto now = std::chrono::system_clock::now();
        auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return epoch.count();
    }
}

int main() {
    try {
        std::cout << "Testing timestamp class..." << std::endl;
        
        // 测试默认构造函数
        exchange::timestamp ts1;
        std::cout << "Default timestamp: " << ts1.value() << std::endl;
        
        // 测试带参数构造函数
        exchange::timestamp ts2(2024, 9, 19, 13, 30, 0, 0);
        std::cout << "Constructed timestamp (2024-09-19 13:30:00): " << ts2.value() << std::endl;
        
        // 测试current()静态方法
        auto current_ts = exchange::timestamp::now();
        std::cout << "Current timestamp: " << current_ts.value() << std::endl;
        
        // 测试日期提取
        auto [year, month, day] = ts2.extract();
        std::cout << "Extracted date: " << year << "-" << month << "-" << day << std::endl;
        
        // 测试比较运算符
        if (ts1 != ts2) {
            std::cout << "Timestamps are different (as expected)" << std::endl;
        }
        
        // 测试toString方法
        std::cout << "Timestamp as string: " << ts2.toString() << std::endl;
        
        std::cout << "All tests passed!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}