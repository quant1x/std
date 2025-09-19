#include <iostream>
#include <vector>
#include <chrono>
#include "../src/timestamp.h"

using namespace exchange;

int main() {
    std::cout << "=== Quant1x C++ Timestamp Demo ===" << std::endl;
    std::cout << std::endl;

    // 1. Basic construction and output
    std::cout << "1. Basic construction:" << std::endl;
    auto ts_now = timestamp::now();
    auto ts_zero = timestamp::zero();
    timestamp ts_custom(1640995200000);  // 2022-01-01 08:00:00 UTC
    
    std::cout << "  Current time: " << ts_now.toString() << std::endl;
    std::cout << "  Zero timestamp: " << ts_zero.toString() << std::endl;
    std::cout << "  Custom time: " << ts_custom.toString() << std::endl;
    std::cout << std::endl;

    // 2. Date-time construction
    std::cout << "2. Date-time construction:" << std::endl;
    timestamp ts_date(2024, 1, 15);
    timestamp ts_datetime(2024, 1, 15, 9, 30, 0);
    timestamp ts_full(2024, 1, 15, 9, 30, 30, 123);
    
    std::cout << "  Date only: " << ts_date.toString() << std::endl;
    std::cout << "  Date time: " << ts_datetime.toString() << std::endl;
    std::cout << "  Full time: " << ts_full.toString() << std::endl;
    std::cout << std::endl;

    // 3. String parsing
    std::cout << "3. String parsing:" << std::endl;
    try {
        auto ts_parsed1 = timestamp::parse("2024-01-15 09:30:00");
        auto ts_parsed2 = timestamp::parse("2024-01-15");
        auto ts_time = timestamp::parse_time("14:30:00");
        
        std::cout << "  Parsed datetime: " << ts_parsed1.toString() << std::endl;
        std::cout << "  Parsed date: " << ts_parsed2.toString() << std::endl;
        std::cout << "  Parsed time (today): " << ts_time.toString() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "  Parse error: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    // 4. Time operations
    std::cout << "4. Time operations:" << std::endl;
    auto base_time = timestamp(2024, 1, 15, 12, 0, 0);
    auto start_day = base_time.start_of_day();
    auto market_time = base_time.pre_market_time();
    auto floor_time = base_time.floor();
    auto ceil_time = base_time.ceil();
    
    std::cout << "  Base time: " << base_time.toString() << std::endl;
    std::cout << "  Start of day: " << start_day.toString() << std::endl;
    std::cout << "  Pre-market time: " << market_time.toString() << std::endl;
    std::cout << "  Floor (0 sec): " << floor_time.toString() << std::endl;
    std::cout << "  Ceil (59 sec): " << ceil_time.toString() << std::endl;
    std::cout << std::endl;

    // 5. Offset operations
    std::cout << "5. Offset operations:" << std::endl;
    auto offset1 = base_time.offset(1, 30, 45, 500);  // +1h 30m 45s 500ms
    auto today_10am = base_time.today(10, 0, 0);
    auto since_10am = base_time.since(10, 0, 0);
    
    std::cout << "  Original: " << base_time.toString() << std::endl;
    std::cout << "  Offset +1h30m45s500ms: " << offset1.toString() << std::endl;
    std::cout << "  Today 10:00:00: " << today_10am.toString() << std::endl;
    std::cout << "  Since 10:00:00: " << since_10am.toString() << std::endl;
    std::cout << std::endl;

    // 6. Format and extract
    std::cout << "6. Format and extract:" << std::endl;
    auto [year, month, day] = base_time.extract();
    
    std::cout << "  Full string: " << base_time.toString() << std::endl;
    std::cout << "  Time only: " << base_time.only_time() << std::endl;
    std::cout << "  Date only: " << base_time.only_date() << std::endl;
    std::cout << "  YYYYMMDD: " << base_time.yyyymmdd() << std::endl;
    std::cout << "  Extracted - Year: " << year << ", Month: " << month << ", Day: " << day << std::endl;
    std::cout << std::endl;

    // 7. Comparison operations
    std::cout << "7. Comparison operations:" << std::endl;
    auto ts1 = timestamp(2024, 1, 15, 9, 0, 0);
    auto ts2 = timestamp(2024, 1, 15, 10, 0, 0);
    auto ts3 = timestamp(2024, 1, 16, 9, 0, 0);
    
    std::cout << "  ts1: " << ts1.toString() << std::endl;
    std::cout << "  ts2: " << ts2.toString() << std::endl;
    std::cout << "  ts3: " << ts3.toString() << std::endl;
    std::cout << "  ts1 < ts2: " << (ts1 < ts2) << std::endl;
    std::cout << "  ts1 == ts2: " << (ts1 == ts2) << std::endl;
    std::cout << "  ts1.is_same_date(ts2): " << ts1.is_same_date(ts2) << std::endl;
    std::cout << "  ts1.is_same_date(ts3): " << ts1.is_same_date(ts3) << std::endl;
    std::cout << std::endl;

    // 8. Value access and conversion
    std::cout << "8. Value access and conversion:" << std::endl;
    std::cout << "  ts1.value(): " << ts1.value() << std::endl;
    std::cout << "  ts1.empty(): " << ts1.empty() << std::endl;
    std::cout << "  ts_zero.empty(): " << ts_zero.empty() << std::endl;
    std::cout << "  Static cast to int64_t: " << static_cast<int64_t>(ts1) << std::endl;
    std::cout << std::endl;

    // 9. Static factory methods
    std::cout << "9. Static factory methods:" << std::endl;
    auto midnight_ts = timestamp::midnight();
    auto premarket_ts = timestamp::pre_market_time(2024, 1, 15);
    
    std::cout << "  Midnight: " << midnight_ts.toString() << std::endl;
    std::cout << "  Pre-market for 2024-01-15: " << premarket_ts.toString() << std::endl;
    std::cout << std::endl;

    // 10. Performance test
    std::cout << "10. Performance test:" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    const int iterations = 100000;
    
    for (int i = 0; i < iterations; ++i) {
        auto ts = timestamp::now();
        auto str = ts.toString();
        (void)str;  // Suppress unused variable warning
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "  " << iterations << " operations in " << duration.count() << " microseconds" << std::endl;
    std::cout << "  Average: " << (duration.count() / static_cast<double>(iterations)) << " microseconds per operation" << std::endl;

    std::cout << std::endl;
    std::cout << "=== Demo completed successfully! ===" << std::endl;
    
    return 0;
}