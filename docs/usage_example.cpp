// 下游用户的使用示例
#include <iostream>
#include <quant1x/timestamp.h>
#include <quant1x/time.h>

int main() {
    // 使用 quant1x 命名空间中的类
    quant1x::timestamp now = quant1x::timestamp::now();
    std::cout << "当前时间戳: " << now.value() << std::endl;
    
    // 使用 api 命名空间中的函数
    std::string date_str = "2024-12-31";
    int64_t parsed_date = api::parse_date(date_str);
    std::cout << "解析日期: " << parsed_date << std::endl;
    
    // 时区转换
    int64_t utc_ms = now.value();
    int64_t local_ms = api::ms_utc_to_local(utc_ms);
    std::cout << "本地时间: " << local_ms << std::endl;
    
    return 0;
}