#pragma once
#ifndef QUANT1X_EXCHANGE_TIMESTAMP_H
#define QUANT1X_EXCHANGE_TIMESTAMP_H 1

#include <chrono>
#include <ctime>
#include <iomanip>
#include <optional>
#include <ostream>
#include <sstream>
#include <tuple>

namespace exchange {

    constexpr const int64_t seconds_per_minute      = 60;
    constexpr const int64_t seconds_per_hour        = 60 * seconds_per_minute;
    constexpr const int64_t seconds_per_day         = 24 * seconds_per_hour;
    constexpr const int64_t milliseconds_per_second = 1000;
    constexpr const int64_t milliseconds_per_minute = seconds_per_minute * milliseconds_per_second;
    constexpr const int64_t milliseconds_per_hour   = seconds_per_hour * milliseconds_per_second;
    constexpr const int64_t milliseconds_per_day    = seconds_per_day * milliseconds_per_second;

    /**
     * @brief Local timestamp in milliseconds
     */
    class timestamp {
    private:
        int64_t ms_;

        /**
         * @brief Current local time
         * @return
         */
        static int64_t current();

    public:
        timestamp();
        timestamp(int64_t t);
        timestamp(const std::chrono::system_clock::time_point &tp);
        // Default accepts date time
        timestamp(const std::string &str);

        /**
         * @brief Construct timestamp from local time
         * @param y year
         * @param m month
         * @param d day
         * @param hh hour
         * @param mm minute
         * @param ss second
         * @param sss millisecond
         */
        timestamp(int y, int m, int d, int hh = 0, int mm = 0, int ss = 0, int sss = 0);

        /**
         * @brief Get milliseconds
         * @return
         */
        int64_t value() const;

        /**
         * @brief Pre-market timestamp, construct timestamp with year/month/day, default time is 9 o'clock
         * @param year year
         * @param month month
         * @param day day
         * @return
         */
        static timestamp pre_market_time(int year, int month, int day);

        /**
         * @brief Current timestamp
         * @return
         */
        static timestamp now();

        // Zero value
        static timestamp zero();

        // Parse date time
        static timestamp parse(const std::string &str);

        // Parse time only
        static timestamp parse_time(const std::string &str);

        // Get start of day (00:00:00.000) timestamp for current timestamp
        timestamp start_of_day() const;

        // Current day midnight
        static timestamp midnight();

        // Today with hour/minute/second/millisecond
        timestamp today(int hour = 0, int minute = 0, int second = 0, int millisecond = 0) const;
        // Today since midnight with hour/minute/second/millisecond
        timestamp since(int hour = 0, int minute = 0, int second = 0, int millisecond = 0) const;
        // Offset current timestamp by hour/minute/second/millisecond
        timestamp offset(int hour = 0, int minute = 0, int second = 0, int millisecond = 0) const;

        // Truncate current timestamp to pre-market time
        timestamp pre_market_time() const;

        // Adjust timestamp to 0 seconds 0 milliseconds (for begin)
        timestamp floor() const;

        // Adjust timestamp to 59 seconds 999 milliseconds (for end)
        timestamp ceil() const;

        // Extract year/month/day
        std::tuple<int, int, int> extract() const;
        // Format timestamp in milliseconds
        std::string toString(const std::string &layout = "{:%Y-%m-%d %H:%M:%S}") const;
        // Truncate milliseconds, format time in seconds, default output hours/minutes/seconds
        std::string toStringAsTimeInSeconds(const std::string &layout = "{:%H:%M:%S}") const;
        // Return date
        std::string only_date() const;
        std::string cache_date() const;
        // Return time
        std::string only_time() const;
        // Return integer date
        uint32_t yyyymmdd() const;
        // Is empty, i.e. zero value
        bool empty() const;

        friend std::ostream &operator<<(std::ostream &os, const timestamp &ts);
        // Assignment operator
        timestamp &operator=(int64_t val) {
            ms_ = val;
            return *this;
        }
        // Type conversion operator
        operator int64_t() const { return value(); }

        /**
         * @brief Check if two timestamps represent the same day (local time)
         * @param other The other timestamp to compare
         * @return true if same day, false otherwise
         */
        bool is_same_date(const timestamp &other) const noexcept;

        bool operator==(const timestamp &rhs) const;
        bool operator!=(const timestamp &rhs) const;
        bool operator<(const timestamp &rhs) const;
        bool operator>(const timestamp &rhs) const;
        bool operator<=(const timestamp &rhs) const;
        bool operator>=(const timestamp &rhs) const;
    };
}  // namespace exchange

#endif  // QUANT1X_EXCHANGE_TIMESTAMP_H