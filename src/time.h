#pragma once
#ifndef QUANT1X_STD_TIME_H
#define QUANT1X_STD_TIME_H 1

#include "base.h"
#include <string>
#include <chrono>

namespace api {

    // Type aliases for cross-platform compatibility
    using i64 = int64_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    // 获取时区偏移的毫秒数
    int64_t zone_offset_milliseconds();
    
    /// 解析日期时间字符串 - 主要用于日期格式，也支持包含时间
    /// 支持格式: "2023-05-15 14:30:00", "2023-05-15", "20230515" 等
    int64_t parse_date(const std::string &str);
    
    /// 解析时间字符串 - 主要用于时间格式，但也兼容完整日期时间
    /// 支持格式: "14:30:00"(纯时间), "2023-05-15 14:30:00"(完整), "143000" 等
    /// 设计目的：用户关注时分秒时使用，但不限制输入格式
    int64_t parse_time(const std::string &str);

    int64_t ms_utc_to_local(const int64_t &milliseconds);

    int64_t ms_local_to_utc(const int64_t &milliseconds);

    std::chrono::system_clock::time_point from_local(const int64_t &milliseconds);
    int64_t from_time_point(const std::chrono::system_clock::time_point& tp);

    /// 获取当前日期的字符串
    std::string today();
    /// 获取当前时间戳的字符串
    std::string get_timestamp();
    /// utc时间 格式化成 本地时间字符串
    std::string to_string(const std::chrono::system_clock::time_point &tp);

    // 获得当前季度的初始和结束日期, months为偏移的月数
    std::pair<std::string, std::string> GetQuarterDay(int months = 0);

    // 通过给定的日期 获得日期所在财报的季度、初始以及结束日期
    //
    // diff 季度偏移数, 大于0前移diff个季度, 小于0后移diff个季度, 默认为当前季度
    std::tuple<std::string, std::string, std::string> GetQuarterByDate(const std::string& date, int diff = 0);

    class [[deprecated("Time类已废弃, 自0.1.0版本起不再推荐使用. 请使用 quant1x::api::timestamp 代替")]] Time {
    private:
        int64_t value_;

    public:
        Time() : value_(0) {}
        explicit Time(int64_t value) : value_(value) {}

        int64_t value() const { return value_; }
    };

} // namespace api

#endif //QUANT1X_STD_TIME_H
