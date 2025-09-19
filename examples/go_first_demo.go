// Go优先设计的timestamp包演示程序
package main

import (
	"fmt"

	"gitee.com/quant1x/std/timestamp"
)

func main() {
	fmt.Println("=== C++为基础的双向奔赴timestamp包演示 ===")

	// 1. C++风格的主要API演示
	fmt.Println("\n1. C++风格API（主要接口）:")

	// 使用C++风格的API作为主要接口
	ts1 := timestamp.NowTimestamp()
	fmt.Printf("当前时间戳: %s\n", ts1.String())

	ts2 := timestamp.NewTimestamp(1640995200000)
	fmt.Printf("从毫秒创建: %s\n", ts2.String())

	// C++风格的比较
	if ts1.Greater(ts2) {
		fmt.Println("ts1 > ts2 (C++风格比较)")
	}

	fmt.Println("\n2. Go原生数据交互:")

	// Go原生的int64时间戳
	rawTime := timestamp.Now()
	fmt.Printf("Go原生时间戳: %d\n", rawTime)
	
	// C++对象与Go原生类型的双向转换
	cppFromGo := timestamp.NewTimestamp(rawTime)
	goFromCpp := cppFromGo.AsTimestamp()
	
	fmt.Printf("C++对象: %s\n", cppFromGo.String())
	fmt.Printf("转回Go: %d\n", goFromCpp)

	fmt.Println("\n3. C++丰富API演示:")
	
	// 创建测试时间戳
	testTime := timestamp.NewTimestampFromDate(2022, 6, 15, 14, 30, 45, 123)
	
	fmt.Println("C++风格的丰富时间操作:")
	fmt.Printf("  完整时间: %s\n", testTime.ToString())
	fmt.Printf("  仅日期: %s\n", testTime.OnlyDate())
	fmt.Printf("  仅时间: %s\n", testTime.OnlyTime())
	fmt.Printf("  YYYYMMDD: %d\n", testTime.YYYYMMDD())
	fmt.Printf("  当天9点: %s\n", testTime.Today(9, 0, 0, 0).String())
	fmt.Printf("  偏移2小时: %s\n", testTime.Offset(2, 0, 0, 0).String())

	fmt.Println("\n=== 演示完成 ===")
	fmt.Println("这展示了C++为基础的双向奔赴设计:")
	fmt.Println("- C++丰富的API作为核心功能")
	fmt.Println("- Go原生int64类型无缝对接")
	fmt.Println("- 保持C++习惯的同时适配Go生态")
	fmt.Println("- 零开销的双向类型转换")
}
