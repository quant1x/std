#include "timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace exchange {

    // 静态方法实现
    int64_t timestamp::current() {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    // 构造函数
    timestamp::timestamp() : ms_(0) {}

    timestamp::timestamp(int64_t t) : ms_(t) {}

    timestamp::timestamp(const std::chrono::system_clock::time_point& tp) {
        auto duration = tp.time_since_epoch();
        ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    timestamp::timestamp(const std::string& str) {
        *this = parse(str);
    }

    timestamp::timestamp(int y, int m, int d, int hh, int mm, int ss, int sss) {
        std::tm tm = {};
        tm.tm_year = y - 1900;
        tm.tm_mon = m - 1;
        tm.tm_mday = d;
        tm.tm_hour = hh;
        tm.tm_min = mm;
        tm.tm_sec = ss;

        auto time = std::mktime(&tm);
        if (time == -1) {
            throw std::invalid_argument("Invalid date/time values");
        }

        ms_ = static_cast<int64_t>(time) * 1000 + sss;
    }

    // 访问器
    int64_t timestamp::value() const {
        return ms_;
    }

    // 静态工厂方法
    timestamp timestamp::pre_market_time(int year, int month, int day) {
        return timestamp(year, month, day, 9, 25, 0, 0);  // 中国市场盘前时间
    }

    timestamp timestamp::now() {
        return timestamp(current());
    }

    timestamp timestamp::zero() {
        return timestamp(0);
    }

    timestamp timestamp::parse(const std::string& str) {
        if (str.empty()) {
            throw std::invalid_argument("Empty string");
        }

        // 尝试解析 "YYYY-MM-DD HH:MM:SS" 格式
        if (str.length() >= 19 && str[4] == '-' && str[7] == '-' && str[13] == ':' && str[16] == ':') {
            int year = std::stoi(str.substr(0, 4));
            int month = std::stoi(str.substr(5, 2));
            int day = std::stoi(str.substr(8, 2));
            int hour = std::stoi(str.substr(11, 2));
            int minute = std::stoi(str.substr(14, 2));
            int second = std::stoi(str.substr(17, 2));
            return timestamp(year, month, day, hour, minute, second, 0);
        }
        // 尝试解析 "YYYY-MM-DD" 格式
        else if (str.length() >= 10 && str[4] == '-' && str[7] == '-') {
            int year = std::stoi(str.substr(0, 4));
            int month = std::stoi(str.substr(5, 2));
            int day = std::stoi(str.substr(8, 2));
            return timestamp(year, month, day, 0, 0, 0, 0);
        }

        throw std::invalid_argument("Invalid date format: " + str);
    }

    timestamp timestamp::parse_time(const std::string& str) {
        if (str.length() >= 8 && str[2] == ':' && str[5] == ':') {
            int hour = std::stoi(str.substr(0, 2));
            int minute = std::stoi(str.substr(3, 2));
            int second = std::stoi(str.substr(6, 2));
            
            // 使用当前日期
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            auto tm = *std::localtime(&time_t);
            
            return timestamp(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, hour, minute, second, 0);
        }
        throw std::invalid_argument("Invalid time format: " + str);
    }

    // 时间操作
    timestamp timestamp::start_of_day() const {
        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        return timestamp(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 0, 0, 0, 0);
    }

    timestamp timestamp::midnight() {
        auto now_ts = timestamp::now();
        return now_ts.start_of_day();
    }

    timestamp timestamp::today(int hour, int minute, int second, int millisecond) const {
        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        return timestamp(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, hour, minute, second, millisecond);
    }

    timestamp timestamp::since(int hour, int minute, int second, int millisecond) const {
        return today(hour, minute, second, millisecond);
    }

    timestamp timestamp::offset(int hour, int minute, int second, int millisecond) const {
        int64_t offset_ms = static_cast<int64_t>(hour) * milliseconds_per_hour +
                           static_cast<int64_t>(minute) * milliseconds_per_minute +
                           static_cast<int64_t>(second) * milliseconds_per_second +
                           static_cast<int64_t>(millisecond);
        return timestamp(ms_ + offset_ms);
    }

    timestamp timestamp::pre_market_time() const {
        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        return timestamp(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 9, 25, 0, 0);
    }

    // 继续实现其他必要的方法...
    bool timestamp::empty() const {
        return ms_ == 0;
    }

    std::tuple<int, int, int> timestamp::extract() const {
        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        return std::make_tuple(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    }

    timestamp timestamp::floor() const {
        return timestamp((ms_ / 1000) * 1000);
    }

    timestamp timestamp::ceil() const {
        int64_t seconds = (ms_ + 999) / 1000;
        return timestamp(seconds * 1000);
    }

    // 格式化方法
    std::string timestamp::toString() const {
        if (empty()) {
            return "1970-01-01 00:00:00";
        }

        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << (tm.tm_year + 1900) << "-"
            << std::setw(2) << (tm.tm_mon + 1) << "-"
            << std::setw(2) << tm.tm_mday << " "
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << ":"
            << std::setw(2) << tm.tm_sec;
        
        return oss.str();
    }

    std::string timestamp::toString(const std::string& layout) const {
        return toString();  // 简化实现
    }

    std::string timestamp::only_date() const {
        if (empty()) {
            return "1970-01-01";
        }

        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << (tm.tm_year + 1900) << "-"
            << std::setw(2) << (tm.tm_mon + 1) << "-"
            << std::setw(2) << tm.tm_mday;
        
        return oss.str();
    }

    std::string timestamp::only_time() const {
        if (empty()) {
            return "00:00:00";
        }

        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << ":"
            << std::setw(2) << tm.tm_sec;
        
        return oss.str();
    }

    std::string timestamp::yyyymmdd() const {
        if (empty()) {
            return "19700101";
        }

        auto time_t = static_cast<std::time_t>(ms_ / 1000);
        auto tm = *std::localtime(&time_t);
        
        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(4) << (tm.tm_year + 1900)
            << std::setw(2) << (tm.tm_mon + 1)
            << std::setw(2) << tm.tm_mday;
        
        return oss.str();
    }

    // 比较操作符
    bool timestamp::operator==(const timestamp& other) const {
        return ms_ == other.ms_;
    }

    bool timestamp::operator!=(const timestamp& other) const {
        return ms_ != other.ms_;
    }

    bool timestamp::operator<(const timestamp& other) const {
        return ms_ < other.ms_;
    }

    bool timestamp::operator<=(const timestamp& other) const {
        return ms_ <= other.ms_;
    }

    bool timestamp::operator>(const timestamp& other) const {
        return ms_ > other.ms_;
    }

    bool timestamp::operator>=(const timestamp& other) const {
        return ms_ >= other.ms_;
    }

    bool timestamp::is_same_date(const timestamp& other) const {
        auto [y1, m1, d1] = extract();
        auto [y2, m2, d2] = other.extract();
        return y1 == y2 && m1 == m2 && d1 == d2;
    }

    // 类型转换
    timestamp::operator int64_t() const {
        return ms_;
    }

    // 流输出
    std::ostream& operator<<(std::ostream& os, const timestamp& ts) {
        return os << ts.toString();
    }

} // namespace exchange