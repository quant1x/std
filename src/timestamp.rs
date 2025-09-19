use chrono::{DateTime, Datelike, Local, NaiveDate, NaiveDateTime, TimeZone, Timelike};
use std::fmt;

// 时间常量
pub const SECONDS_PER_MINUTE: i64 = 60;
pub const SECONDS_PER_HOUR: i64 = 60 * SECONDS_PER_MINUTE;
pub const SECONDS_PER_DAY: i64 = 24 * SECONDS_PER_HOUR;
pub const MILLISECONDS_PER_SECOND: i64 = 1000;
pub const MILLISECONDS_PER_MINUTE: i64 = SECONDS_PER_MINUTE * MILLISECONDS_PER_SECOND;
pub const MILLISECONDS_PER_HOUR: i64 = SECONDS_PER_HOUR * MILLISECONDS_PER_SECOND;
pub const MILLISECONDS_PER_DAY: i64 = SECONDS_PER_DAY * MILLISECONDS_PER_SECOND;

// 盘前时间配置（对应C++中的config::cn_pre_market_*）
pub const PRE_MARKET_HOUR: u32 = 9;
pub const PRE_MARKET_MINUTE: u32 = 0;
pub const PRE_MARKET_SECOND: u32 = 0;

/// 本地时间戳，单位毫秒
/// 
/// 这个结构体提供了与C++ timestamp类相同的API和功能，
/// 用于处理本地时间戳的各种操作。
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct Timestamp {
    ms: i64,
}

impl Timestamp {
    /// 创建新的时间戳
    /// 
    /// # Arguments
    /// * `ms` - 毫秒时间戳
    /// 
    /// # Examples
    /// ```
    /// use quant1x_std::Timestamp;
    /// let ts = Timestamp::new(1640995200000);
    /// ```
    pub fn new(ms: i64) -> Self {
        Self { ms }
    }

    /// 从chrono::DateTime创建时间戳
    /// 
    /// # Arguments
    /// * `dt` - chrono::DateTime对象
    pub fn from_datetime(dt: DateTime<Local>) -> Self {
        Self::new(dt.timestamp_millis())
    }

    /// 从字符串解析创建时间戳
    /// 
    /// # Arguments
    /// * `s` - 时间字符串
    pub fn from_string(s: &str) -> Result<Self, chrono::ParseError> {
        Self::parse(s)
    }

    /// 从年月日时分秒毫秒创建时间戳
    /// 
    /// # Arguments
    /// * `year` - 年
    /// * `month` - 月
    /// * `day` - 日
    /// * `hour` - 时
    /// * `minute` - 分
    /// * `second` - 秒
    /// * `millisecond` - 毫秒
    pub fn from_date(
        year: i32,
        month: u32,
        day: u32,
        hour: u32,
        minute: u32,
        second: u32,
        millisecond: u32,
    ) -> Option<Self> {
        let naive_date = NaiveDate::from_ymd_opt(year, month, day)?;
        let naive_time = naive_date
            .and_hms_milli_opt(hour, minute, second, millisecond)?;
        let dt = Local.from_local_datetime(&naive_time).single()?;
        Some(Self::from_datetime(dt))
    }

    /// 获取毫秒时间戳值
    pub fn value(&self) -> i64 {
        self.ms
    }

    /// 创建盘前时间戳
    /// 
    /// # Arguments
    /// * `year` - 年
    /// * `month` - 月
    /// * `day` - 日
    pub fn pre_market_time(year: i32, month: u32, day: u32) -> Option<Self> {
        Self::from_date(year, month, day, PRE_MARKET_HOUR, PRE_MARKET_MINUTE, PRE_MARKET_SECOND, 0)
    }

    /// 当前时间戳
    pub fn now() -> Self {
        Self::from_datetime(Local::now())
    }

    /// 零值时间戳
    pub fn zero() -> Self {
        Self::new(0)
    }

    /// 解析日期时间字符串
    /// 
    /// 支持多种格式：
    /// - "2022-01-01 15:30:45"
    /// - "2022-01-01"
    /// - "2022/01/01 15:30:45"
    pub fn parse(s: &str) -> Result<Self, chrono::ParseError> {
        // 尝试多种格式解析
        let formats = [
            "%Y-%m-%d %H:%M:%S%.f",
            "%Y-%m-%d %H:%M:%S",
            "%Y-%m-%d",
            "%Y/%m/%d %H:%M:%S%.f",
            "%Y/%m/%d %H:%M:%S",
            "%Y/%m/%d",
        ];

        for format in &formats {
            if let Ok(naive_dt) = NaiveDateTime::parse_from_str(s, format) {
                if let Some(dt) = Local.from_local_datetime(&naive_dt).single() {
                    return Ok(Self::from_datetime(dt));
                }
            }
        }

        // 尝试仅日期格式
        for format in &["%Y-%m-%d", "%Y/%m/%d"] {
            if let Ok(naive_date) = NaiveDate::parse_from_str(s, format) {
                if let Some(naive_dt) = naive_date.and_hms_opt(0, 0, 0) {
                    if let Some(dt) = Local.from_local_datetime(&naive_dt).single() {
                        return Ok(Self::from_datetime(dt));
                    }
                }
            }
        }

        // 如果所有格式都失败，创建一个错误
        Err(chrono::NaiveDateTime::parse_from_str("invalid", "%Y-%m-%d").unwrap_err())
    }

    /// 解析时间字符串（仅时间部分）
    /// 
    /// # Arguments
    /// * `s` - 时间字符串，如 "14:30:45"
    pub fn parse_time(s: &str) -> Result<Self, chrono::ParseError> {
        let time_formats = ["%H:%M:%S%.f", "%H:%M:%S", "%H:%M"];
        
        for format in &time_formats {
            if let Ok(time) = chrono::NaiveTime::parse_from_str(s, format) {
                let today = Local::now().date_naive();
                if let Some(dt) = today.and_time(time).and_local_timezone(Local).single() {
                    return Ok(Self::from_datetime(dt));
                }
            }
        }
        
        Err(chrono::NaiveTime::parse_from_str("invalid", "%H:%M:%S").unwrap_err())
    }

    /// 获取当天零点的时间戳
    pub fn start_of_day(&self) -> Self {
        let dt = self.to_datetime();
        let start_of_day = dt.date_naive().and_hms_opt(0, 0, 0).unwrap();
        let start_dt = Local.from_local_datetime(&start_of_day).single().unwrap();
        Self::from_datetime(start_dt)
    }

    /// 当前时间的零点整
    pub fn midnight() -> Self {
        Self::now().start_of_day()
    }

    /// 今天的指定时间
    /// 
    /// # Arguments
    /// * `hour` - 时
    /// * `minute` - 分
    /// * `second` - 秒
    /// * `millisecond` - 毫秒
    pub fn today(&self, hour: u32, minute: u32, second: u32, millisecond: u32) -> Option<Self> {
        let dt = self.to_datetime();
        let date = dt.date_naive();
        let new_time = date.and_hms_milli_opt(hour, minute, second, millisecond)?;
        let new_dt = Local.from_local_datetime(&new_time).single()?;
        Some(Self::from_datetime(new_dt))
    }

    /// 当天自0点开始的偏移时间
    /// 
    /// # Arguments
    /// * `hour` - 小时偏移
    /// * `minute` - 分钟偏移
    /// * `second` - 秒偏移
    /// * `millisecond` - 毫秒偏移
    pub fn since(&self, hour: u32, minute: u32, second: u32, millisecond: u32) -> Self {
        let start_of_day = self.start_of_day();
        start_of_day.offset(hour as i32, minute as i32, second as i32, millisecond as i32)
    }

    /// 偏移指定时间
    /// 
    /// # Arguments
    /// * `hour` - 小时偏移
    /// * `minute` - 分钟偏移
    /// * `second` - 秒偏移
    /// * `millisecond` - 毫秒偏移
    pub fn offset(&self, hour: i32, minute: i32, second: i32, millisecond: i32) -> Self {
        let offset_ms = hour as i64 * MILLISECONDS_PER_HOUR
            + minute as i64 * MILLISECONDS_PER_MINUTE
            + second as i64 * MILLISECONDS_PER_SECOND
            + millisecond as i64;
        Self::new(self.ms + offset_ms)
    }

    /// 转换为盘前时间
    pub fn pre_market_time_from_current(&self) -> Option<Self> {
        let _dt = self.to_datetime();
        self.today(PRE_MARKET_HOUR, PRE_MARKET_MINUTE, PRE_MARKET_SECOND, 0)
    }

    /// 调整到分钟的开始（秒和毫秒归零）
    pub fn floor(&self) -> Self {
        let dt = self.to_datetime();
        let floored = dt
            .with_second(0).unwrap()
            .with_nanosecond(0).unwrap();
        Self::from_datetime(floored)
    }

    /// 调整到分钟的结束（59秒999毫秒）
    pub fn ceil(&self) -> Self {
        let dt = self.to_datetime();
        let ceiled = dt
            .with_second(59).unwrap()
            .with_nanosecond(999_000_000).unwrap();
        Self::from_datetime(ceiled)
    }

    /// 提取年月日
    pub fn extract(&self) -> (i32, u32, u32) {
        let dt = self.to_datetime();
        (dt.year(), dt.month(), dt.day())
    }

    /// 格式化为字符串
    /// 
    /// # Arguments
    /// * `layout` - 格式字符串，默认为 "%Y-%m-%d %H:%M:%S"
    pub fn to_string_with_layout(&self, layout: &str) -> String {
        let dt = self.to_datetime();
        dt.format(layout).to_string()
    }

    /// 格式化为字符串（以秒为单位，截断毫秒）
    /// 
    /// # Arguments
    /// * `layout` - 格式字符串，默认为 "%H:%M:%S"
    pub fn to_string_as_time_in_seconds(&self, layout: &str) -> String {
        let dt = self.to_datetime();
        // 截断到秒
        let truncated = dt.with_nanosecond(0).unwrap();
        truncated.format(layout).to_string()
    }

    /// 返回仅日期部分
    pub fn only_date(&self) -> String {
        self.to_string_with_layout("%Y-%m-%d")
    }

    /// 返回缓存日期格式
    pub fn cache_date(&self) -> String {
        self.to_string_with_layout("%Y%m%d")
    }

    /// 返回仅时间部分
    pub fn only_time(&self) -> String {
        self.to_string_with_layout("%H:%M:%S")
    }

    /// 返回YYYYMMDD格式的整数
    pub fn yyyymmdd(&self) -> u32 {
        let dt = self.to_datetime();
        (dt.year() as u32) * 10000 + dt.month() * 100 + dt.day()
    }

    /// 是否为空（零值）
    pub fn is_empty(&self) -> bool {
        self.ms == 0
    }

    /// 检查是否与另一个时间戳在同一天
    /// 
    /// # Arguments
    /// * `other` - 另一个时间戳
    pub fn is_same_date(&self, other: &Self) -> bool {
        let dt1 = self.to_datetime();
        let dt2 = other.to_datetime();
        dt1.date_naive() == dt2.date_naive()
    }

    /// 转换为chrono::DateTime
    pub fn to_datetime(&self) -> DateTime<Local> {
        Local.timestamp_millis_opt(self.ms).single().unwrap_or_else(|| {
            // 如果转换失败，返回epoch时间
            Local.timestamp_millis_opt(0).single().unwrap()
        })
    }

    /// 转换为Unix毫秒时间戳
    pub fn unix_millis(&self) -> i64 {
        self.ms
    }
}

// 实现Display trait，对应C++的operator<<
impl fmt::Display for Timestamp {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.to_string_with_layout("%Y-%m-%d %H:%M:%S%.3f"))
    }
}

// 实现From trait用于类型转换
impl From<i64> for Timestamp {
    fn from(ms: i64) -> Self {
        Self::new(ms)
    }
}

impl From<Timestamp> for i64 {
    fn from(ts: Timestamp) -> Self {
        ts.value()
    }
}

impl From<DateTime<Local>> for Timestamp {
    fn from(dt: DateTime<Local>) -> Self {
        Self::from_datetime(dt)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_timestamp_creation() {
        let ts = Timestamp::new(1640995200000);
        assert_eq!(ts.value(), 1640995200000);
    }

    #[test]
    fn test_timestamp_now() {
        let ts = Timestamp::now();
        assert!(ts.value() > 0);
    }

    #[test]
    fn test_timestamp_zero() {
        let ts = Timestamp::zero();
        assert_eq!(ts.value(), 0);
        assert!(ts.is_empty());
    }

    #[test]
    fn test_timestamp_from_date() {
        let ts = Timestamp::from_date(2022, 1, 1, 0, 0, 0, 0).unwrap();
        let (year, month, day) = ts.extract();
        assert_eq!(year, 2022);
        assert_eq!(month, 1);
        assert_eq!(day, 1);
    }

    #[test]
    fn test_timestamp_parse() {
        let ts = Timestamp::parse("2022-06-15 14:30:45").unwrap();
        let (year, month, day) = ts.extract();
        assert_eq!(year, 2022);
        assert_eq!(month, 6);
        assert_eq!(day, 15);
    }

    #[test]
    fn test_timestamp_formatting() {
        let ts = Timestamp::from_date(2022, 6, 15, 14, 30, 45, 123).unwrap();
        assert_eq!(ts.only_date(), "2022-06-15");
        assert_eq!(ts.only_time(), "14:30:45");
        assert_eq!(ts.yyyymmdd(), 20220615);
    }

    #[test]
    fn test_timestamp_operations() {
        let ts = Timestamp::from_date(2022, 6, 15, 14, 30, 45, 123).unwrap();
        
        // 测试今天9点
        let nine_am = ts.today(9, 0, 0, 0).unwrap();
        assert_eq!(nine_am.only_time(), "09:00:00");
        
        // 测试偏移2小时
        let offset = ts.offset(2, 0, 0, 0);
        assert_eq!(offset.only_time(), "16:30:45");
    }

    #[test]
    fn test_timestamp_comparison() {
        let ts1 = Timestamp::new(1000);
        let ts2 = Timestamp::new(2000);
        
        assert!(ts1 < ts2);
        assert!(ts2 > ts1);
        assert_eq!(ts1, ts1);
        assert_ne!(ts1, ts2);
    }

    #[test]
    fn test_same_date() {
        let ts1 = Timestamp::from_date(2022, 6, 15, 8, 0, 0, 0).unwrap();
        let ts2 = Timestamp::from_date(2022, 6, 15, 20, 0, 0, 0).unwrap();
        let ts3 = Timestamp::from_date(2022, 6, 16, 8, 0, 0, 0).unwrap();
        
        assert!(ts1.is_same_date(&ts2));
        assert!(!ts1.is_same_date(&ts3));
    }
}