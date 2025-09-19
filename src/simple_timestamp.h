#pragma once
#include <chrono>
#include <ctime>
#include <string>
#include <tuple>
#include <ostream>
#include <sstream>
#include <iomanip>

namespace exchange {

    constexpr const int64_t seconds_per_minute      = 60;
    constexpr const int64_t seconds_per_hour        = 60 * seconds_per_minute;
    constexpr const int64_t seconds_per_day         = 24 * seconds_per_hour;
    constexpr const int64_t milliseconds_per_second = 1000;
    constexpr const int64_t milliseconds_per_minute = seconds_per_minute * milliseconds_per_second;
    constexpr const int64_t milliseconds_per_hour   = seconds_per_hour * milliseconds_per_second;
    constexpr const int64_t milliseconds_per_day    = seconds_per_day * milliseconds_per_second;

    class timestamp {
    private:
        int64_t ms_;

        static int64_t current() {
            auto utc_now = std::chrono::system_clock::now();
            auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(utc_now.time_since_epoch());
            return epoch.count(); // 简化：不做时区转换
        }

    public:
        timestamp() : ms_(current()) {}
        timestamp(int64_t t) : ms_(t) {}
        timestamp(const std::chrono::system_clock::time_point &tp) : ms_(0) {
            auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
            ms_ = epoch.count(); // 简化：不做时区转换
        }
        
        timestamp(int y, int m, int d, int hh = 0, int mm = 0, int ss = 0, int sss = 0) : ms_(0) {
            // 使用标准库创建时间点
            std::tm tm = {};
            tm.tm_year = y - 1900;
            tm.tm_mon = m - 1;
            tm.tm_mday = d;
            tm.tm_hour = hh;
            tm.tm_min = mm;
            tm.tm_sec = ss;
            tm.tm_isdst = -1;
            
            auto time_t_val = std::mktime(&tm);
            if (time_t_val != -1) {
                ms_ = static_cast<int64_t>(time_t_val) * 1000 + sss;
            }
        }

        int64_t value() const { return ms_; }
        
        static timestamp now() {
            return timestamp{current()};
        }
        
        static timestamp zero() {
            return timestamp{0};
        }
        
        std::tuple<int, int, int> extract() const {
            auto seconds = static_cast<std::time_t>(ms_ / milliseconds_per_second);
            std::tm local_time = {};
#ifdef _WIN32
            localtime_s(&local_time, &seconds);
#else
            localtime_r(&seconds, &local_time);
#endif
            int year = local_time.tm_year + 1900;
            int month = local_time.tm_mon + 1;
            int day = local_time.tm_mday;
            return {year, month, day};
        }
        
        std::string toString(const std::string& layout = "{:%Y-%m-%d %H:%M:%S}") const {
            auto seconds = static_cast<std::time_t>(ms_ / milliseconds_per_second);
            std::tm local_time = {};
#ifdef _WIN32
            localtime_s(&local_time, &seconds);
#else
            localtime_r(&seconds, &local_time);
#endif
            
            std::ostringstream oss;
            oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }
        
        bool empty() const { return ms_ == 0; }
        
        bool operator==(const timestamp &rhs) const { return ms_ == rhs.ms_; }
        bool operator!=(const timestamp &rhs) const { return ms_ != rhs.ms_; }
        bool operator<(const timestamp &rhs) const { return ms_ < rhs.ms_; }
        bool operator>(const timestamp &rhs) const { return ms_ > rhs.ms_; }
        bool operator<=(const timestamp &rhs) const { return ms_ <= rhs.ms_; }
        bool operator>=(const timestamp &rhs) const { return ms_ >= rhs.ms_; }
        
        friend std::ostream &operator<<(std::ostream &os, const timestamp &ts) {
            os << ts.toString();
            return os;
        }
    };
}