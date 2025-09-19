// Go优先设计的timestamp包演示程序
package main

import (
	"fmt"

	"gitee.com/quant1x/std/timestamp"
)

func main() {
	fmt.Println("=== Go优先设计的timestamp包演示 ===\n")

	// 1. Go原生API演示
	fmt.Println("1. Go原生API:")

	// 使用Go风格的API
	ts1 := timestamp.Now()
	goTs1 := timestamp.V1Timestamp(ts1)
	fmt.Printf("当前时间戳(Go): %s\n", goTs1.String())

	goTs2 := timestamp.V1Timestamp(1640995200000)
	fmt.Printf("从毫秒创建(Go): %s\n", goTs2.String())

	// Go风格的比较
	if goTs1 > goTs2 {
		fmt.Println("ts1 > ts2 (Go原生比较)")
	}

	fmt.Println("\n2. C++兼容层API:")

	// 使用C++风格的API（兼容层）
	cppTs1 := timestamp.NowTimestamp()
	fmt.Printf("当前时间戳(C++风格): %s\n", cppTs1.String())

	cppTs2 := timestamp.NewTimestamp(1640995200000)
	fmt.Printf("从毫秒创建(C++风格): %s\n", cppTs2.String())

	// C++风格的比较
	if cppTs1.Greater(cppTs2) {
		fmt.Println("cppTs1.Greater(cppTs2) (C++风格比较)")
	}

	fmt.Println("\n3. 混合使用演示:")

	// Go和C++风格混合使用
	goNow := timestamp.Now()
	goTs := timestamp.V1Timestamp(goNow)
	cppTs := timestamp.FromTimestamp(goTs) // Go -> C++风格
	backToGo := cppTs.AsTimestamp()        // C++风格 -> Go

	fmt.Printf("Go原生: %s\n", goTs.String())
	fmt.Printf("转换为C++风格: %s\n", cppTs.String())
	fmt.Printf("再转回Go: %s\n", backToGo.String())

	// 验证一致性
	if goTs == backToGo {
		fmt.Println("✓ 转换保持一致性")
	}

	fmt.Println("\n4. 功能对比演示:")

	// 创建测试时间戳
	testTime := timestamp.NewTimestampFromDate(2022, 6, 15, 14, 30, 45, 123)

	fmt.Println("C++风格的丰富API:")
	fmt.Printf("  完整时间: %s\n", testTime.ToString())
	fmt.Printf("  仅日期: %s\n", testTime.OnlyDate())
	fmt.Printf("  仅时间: %s\n", testTime.OnlyTime())
	fmt.Printf("  YYYYMMDD: %d\n", testTime.YYYYMMDD())
	fmt.Printf("  当天9点: %s\n", testTime.Today(9, 0, 0, 0).String())
	fmt.Printf("  偏移2小时: %s\n", testTime.Offset(2, 0, 0, 0).String())

	fmt.Println("\n5. 性能考虑:")

	// 展示不同API的使用场景
	fmt.Println("Go原生API:")
	fmt.Println("  - 适合Go项目的日常使用")
	fmt.Println("  - 性能最优")
	fmt.Println("  - 符合Go语言习惯")

	fmt.Println("C++兼容层API:")
	fmt.Println("  - 适合从C++代码迁移")
	fmt.Println("  - 提供丰富的时间操作方法")
	fmt.Println("  - 保持C++代码的可读性")

	fmt.Println("\n6. 跨语言一致性验证:")

	// 模拟跨语言的数据交换
	milliseconds := int64(1655293845123) // 2022-06-15 14:30:45.123

	// Go处理
	goResult := timestamp.Time(milliseconds).Format("2006-01-02 15:04:05")

	// C++风格处理
	cppResult := timestamp.NewTimestamp(milliseconds).ToString("2006-01-02 15:04:05")

	fmt.Printf("Go处理结果: %s\n", goResult)
	fmt.Printf("C++风格处理结果: %s\n", cppResult)

	if goResult == cppResult {
		fmt.Println("✓ 跨语言处理结果一致")
	}

	fmt.Println("\n=== 演示完成 ===")
	fmt.Println("这展示了Go优先设计的灵活性:")
	fmt.Println("- Go开发者可以使用原生API获得最佳性能")
	fmt.Println("- C++开发者可以使用兼容层保持代码习惯")
	fmt.Println("- 两种API可以无缝互操作")
	fmt.Println("- 保证了跨语言的一致性")
}
