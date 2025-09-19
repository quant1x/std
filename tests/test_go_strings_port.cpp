#include <iostream>
#include <vector>
#include <cassert>
#include "../src/strings.h"

void test_basic_char_functions() {
    std::cout << "=== 测试基础字符函数 ===" << std::endl;
    
    assert(strings::is_lower('a') == true);
    assert(strings::is_lower('A') == false);
    assert(strings::is_upper('A') == true);
    assert(strings::is_upper('a') == false);
    
    assert(strings::to_lower('A') == 'a');
    assert(strings::to_lower('a') == 'a');
    assert(strings::to_upper('a') == 'A');
    assert(strings::to_upper('A') == 'A');
    
    assert(strings::is_space(' ') == true);
    assert(strings::is_space('\t') == true);
    assert(strings::is_space('a') == false);
    
    assert(strings::is_delimiter('-') == true);
    assert(strings::is_delimiter('_') == true);
    assert(strings::is_delimiter(' ') == true);
    assert(strings::is_delimiter('a') == false);
    
    std::cout << "✅ 基础字符函数测试通过" << std::endl;
}

void test_toString_functions() {
    std::cout << "=== 测试 toString 函数 ===" << std::endl;
    
    assert(strings::toString(int8_t(42)) == "42");
    assert(strings::toString(int32_t(-123)) == "-123");
    assert(strings::toString(uint64_t(999)) == "999");
    assert(strings::toString(true) == "true");
    assert(strings::toString(false) == "false");
    
    // 浮点数测试 (近似比较)
    std::string float_result = strings::toString(3.14f);
    assert(float_result.find("3.14") == 0);
    
    std::cout << "✅ toString 函数测试通过" << std::endl;
}

void test_camel_case() {
    std::cout << "=== 测试 CamelCase 转换 ===" << std::endl;
    
    assert(strings::to_camel_case("hello-world") == "helloWorld");
    assert(strings::to_camel_case("hello_world") == "helloWorld");
    assert(strings::to_camel_case("hello-world-test") == "helloWorldTest");
    
    assert(strings::upper_camel_case("hello world") == "HelloWorld");
    assert(strings::upper_camel_case("hello-world") == "HelloWorld");
    assert(strings::upper_camel_case("hello_world") == "HelloWorld");
    
    assert(strings::lower_camel_case("Hello World") == "helloWorld");
    assert(strings::lower_camel_case("HELLO-WORLD") == "helloWorld");
    
    std::cout << "✅ CamelCase 转换测试通过" << std::endl;
}

void test_snake_case() {
    std::cout << "=== 测试 SnakeCase 转换 ===" << std::endl;
    
    assert(strings::snake_case("HelloWorld") == "hello_world");
    assert(strings::snake_case("helloWorld") == "hello_world");
    assert(strings::snake_case("Hello World") == "hello_world");
    assert(strings::snake_case("XMLHttpRequest") == "xml_http_request");
    
    assert(strings::upper_snake_case("HelloWorld") == "HELLO_WORLD");
    assert(strings::upper_snake_case("helloWorld") == "HELLO_WORLD");
    
    std::cout << "✅ SnakeCase 转换测试通过" << std::endl;
}

void test_kebab_case() {
    std::cout << "=== 测试 KebabCase 转换 ===" << std::endl;
    
    assert(strings::kebab_case("HelloWorld") == "hello-world");
    assert(strings::kebab_case("helloWorld") == "hello-world");
    assert(strings::kebab_case("Hello World") == "hello-world");
    
    assert(strings::upper_kebab_case("HelloWorld") == "HELLO-WORLD");
    assert(strings::upper_kebab_case("helloWorld") == "HELLO-WORLD");
    
    std::cout << "✅ KebabCase 转换测试通过" << std::endl;
}

void test_string_matching() {
    std::cout << "=== 测试字符串匹配函数 ===" << std::endl;
    
    std::vector<std::string> prefixes = {"hello", "hi", "hey"};
    assert(strings::starts_with("hello world", prefixes) == true);
    assert(strings::starts_with("hi there", prefixes) == true);
    assert(strings::starts_with("goodbye", prefixes) == false);
    assert(strings::starts_with("", prefixes) == false);
    
    std::vector<std::string> suffixes = {"world", "test", ".txt"};
    assert(strings::ends_with("hello world", suffixes) == true);
    assert(strings::ends_with("my test", suffixes) == true);
    assert(strings::ends_with("file.txt", suffixes) == true);
    assert(strings::ends_with("hello", suffixes) == false);
    
    assert(strings::is_empty("") == true);
    assert(strings::is_empty("   ") == true);
    assert(strings::is_empty("\t\n\r") == true);  // 实际的控制字符
    assert(strings::is_empty("hello") == false);
    assert(strings::is_empty("  hello  ") == false);
    
    std::cout << "✅ 字符串匹配函数测试通过" << std::endl;
}

int main() {
    std::cout << "🚀 开始测试 strings 模块的 Go 移植功能..." << std::endl;
    
    try {
        test_basic_char_functions();
        test_toString_functions();
        test_camel_case();
        test_snake_case();
        test_kebab_case();
        test_string_matching();
        
        std::cout << std::endl << "🎉 所有 strings 功能测试通过！Go 代码成功移植到 C++！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}