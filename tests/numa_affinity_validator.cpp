// NUMA亲和性功能验证程序
#include "../src/affinity.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <iomanip>
#include <atomic>
#include <numeric>

using namespace api;
using namespace std::chrono;

// 简单的测试框架宏
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            std::cerr << "❌ 断言失败: " << message << std::endl; \
            return false; \
        } else { \
            std::cout << "✅ " << message << std::endl; \
        } \
    } while(0)

// 用简单的函数替代宏，避免字符串处理问题
inline bool expect_no_error(const std::error_code& ec, const char* message) {
    if (ec) {
        std::cerr << "❌ 错误: " << message << " - " << ec.message() << std::endl;
        return false;
    }
    return true;
}

class NumaAffinityValidator {
private:
    NumaTopology topology;
    
public:
    bool initialize() {
        std::cout << "\n=== 初始化NUMA亲和性测试 ===" << std::endl;
        
        std::error_code ec;
        topology = get_numa_topology(ec);
        
        if (ec || topology.node_count == 0) {
            std::cout << "⚠️  未检测到NUMA或获取失败，使用模拟拓扑" << std::endl;
            topology.node_count = 1;
            topology.node_cpus.resize(1);
            topology.cpu_to_node.resize(std::thread::hardware_concurrency(), 0);
            topology.node_memory_sizes.resize(1, 1024);
            topology.is_numa_available = false;
            
            for (unsigned i = 0; i < std::thread::hardware_concurrency(); ++i) {
                topology.node_cpus[0].push_back(i);
            }
        }
        
        std::cout << "系统信息:" << std::endl;
        std::cout << "  NUMA节点数: " << topology.node_count << std::endl;
        std::cout << "  NUMA可用: " << (topology.is_numa_available ? "是" : "否") << std::endl;
        std::cout << "  总CPU数: " << topology.cpu_to_node.size() << std::endl;
        
        for (unsigned node = 0; node < topology.node_count; ++node) {
            std::cout << "  节点 " << node << ": " 
                     << topology.node_cpus[node].size() << " CPUs, "
                     << topology.node_memory_sizes[node] << " MB" << std::endl;
        }
        
        return true;
    }
    
    bool test_basic_cpu_affinity() {
        std::cout << "\n=== 测试基础CPU亲和性 ===" << std::endl;
        
        std::error_code ec;
        
        // 测试绑定当前线程到最优CPU
        bool result = bind_current_thread_to_optimal_cpu(ec);
        TEST_ASSERT(result, "bind_current_thread_to_optimal_cpu 成功");
        if (!expect_no_error(ec, "Bind current thread to optimal CPU")) return false;
        
        // 测试绑定到指定CPU
        if (topology.cpu_to_node.size() > 1) {
            unsigned target_cpu = 1;
            result = bind_current_thread_to_cpu(target_cpu, ec);
            TEST_ASSERT(result, "bind_current_thread_to_cpu 成功");
            if (!expect_no_error(ec, "Bind to specific CPU")) return false;
        }
        
        return true;
    }
    
    bool test_numa_topology() {
        std::cout << "\n=== 测试NUMA拓扑发现 ===" << std::endl;
        
        TEST_ASSERT(topology.node_count > 0, "NUMA节点数大于0");
        TEST_ASSERT(topology.node_cpus.size() == topology.node_count, "节点CPU列表大小匹配");
        TEST_ASSERT(topology.node_memory_sizes.size() == topology.node_count, "节点内存大小列表匹配");
        
        // 验证CPU到节点的映射
        for (size_t cpu = 0; cpu < topology.cpu_to_node.size(); ++cpu) {
            unsigned node = topology.cpu_to_node[cpu];
            TEST_ASSERT(node < topology.node_count, "CPU映射到有效节点");
            
            const auto& node_cpus = topology.node_cpus[node];
            bool found = std::find(node_cpus.begin(), node_cpus.end(), cpu) != node_cpus.end();
            TEST_ASSERT(found, "CPU在对应节点的CPU列表中");
        }
        
        return true;
    }
    
    bool test_numa_cpu_allocator() {
        std::cout << "\n=== 测试NUMA感知CPU分配器 ===" << std::endl;
        
        NumaAwareCpuAllocator allocator(CpuAllocationStrategy::NUMA_LOCAL);
        std::error_code ec;
        
        // 测试普通线程分配
        unsigned cpu1 = allocator.allocate_optimal_cpu(ThreadPriority::NORMAL, nullptr, &ec);
        if (!expect_no_error(ec, "Allocate normal priority CPU")) return false;
        TEST_ASSERT(cpu1 < topology.cpu_to_node.size(), "分配的CPU ID有效");
        
        // 测试高优先级线程分配
        unsigned cpu2 = allocator.allocate_optimal_cpu(ThreadPriority::HIGH_FREQUENCY, nullptr, &ec);
        if (!expect_no_error(ec, "Allocate high frequency CPU")) return false;
        TEST_ASSERT(cpu2 < topology.cpu_to_node.size(), "分配的CPU ID有效");
        
        // 测试隔离CPU分配
        unsigned isolated_cpu = allocator.allocate_isolated_cpu(ec);
        if (!ec) {
            TEST_ASSERT(isolated_cpu < topology.cpu_to_node.size(), "隔离CPU ID有效");
            std::cout << "✅ 成功分配隔离CPU: " << isolated_cpu << std::endl;
        } else {
            std::cout << "⚠️  隔离CPU不可用（正常情况）" << std::endl;
        }
        
        // 验证分配统计
        auto stats = allocator.get_allocation_stats();
        TEST_ASSERT(stats.total_allocations >= 2, "总分配次数至少为2");
        TEST_ASSERT(stats.node_allocations.size() == topology.node_count, "节点分配统计大小匹配");
        
        std::cout << "✅ CPU分配统计: 总计=" << stats.total_allocations 
                  << ", 隔离=" << stats.isolated_allocations << std::endl;
        
        return true;
    }
    
    bool test_numa_memory_allocator() {
        std::cout << "\n=== 测试NUMA感知内存分配器 ===" << std::endl;
        
        NumaAwareAllocator<int> allocator;
        std::error_code ec;
        const size_t test_size = 1000;
        
        // 测试本地内存分配
        int* local_memory = allocator.allocate_local(test_size, ec);
        if (!ec) {
            TEST_ASSERT(local_memory != nullptr, "本地内存分配成功");
            
            // 测试内存读写
            for (size_t i = 0; i < test_size; ++i) {
                local_memory[i] = static_cast<int>(i);
            }
            
            // 验证数据
            bool data_correct = true;
            for (size_t i = 0; i < test_size; ++i) {
                if (local_memory[i] != static_cast<int>(i)) {
                    data_correct = false;
                    break;
                }
            }
            TEST_ASSERT(data_correct, "内存数据读写验证");
            
            allocator.deallocate_numa(local_memory, test_size, ec);
            if (!expect_no_error(ec, "Release local memory")) return false;
            
            std::cout << "✅ 本地内存分配测试通过 (" << test_size << " 个int)" << std::endl;
        }
        
        // 测试标准allocator接口
        try {
            int* std_memory = allocator.allocate(test_size);
            TEST_ASSERT(std_memory != nullptr, "标准分配接口成功");
            
            std_memory[0] = 42;
            std_memory[test_size - 1] = 99;
            TEST_ASSERT(std_memory[0] == 42 && std_memory[test_size - 1] == 99, "标准接口数据读写");
            
            allocator.deallocate(std_memory, test_size);
            std::cout << "✅ 标准allocator接口兼容性验证通过" << std::endl;
            
        } catch (const std::bad_alloc&) {
            std::cout << "⚠️  标准allocator接口内存不足（正常情况）" << std::endl;
        }
        
        return true;
    }
    
    bool test_multithread_cpu_binding() {
        std::cout << "\n=== 测试多线程CPU绑定 ===" << std::endl;
        
        const int num_threads = std::min(4, static_cast<int>(std::thread::hardware_concurrency()));
        std::vector<std::thread> threads;
        std::atomic<int> success_count{0};
        std::atomic<int> error_count{0};
        
        NumaAwareCpuAllocator allocator(CpuAllocationStrategy::NUMA_LOCAL);
        
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([&allocator, &success_count, &error_count]() {
                std::error_code ec;
                
                unsigned cpu = allocator.allocate_optimal_cpu(ThreadPriority::NORMAL, nullptr, &ec);
                if (!ec) {
                    if (bind_current_thread_to_cpu(cpu, ec)) {
                        success_count.fetch_add(1);
                        
                        // 模拟工作负载
                        volatile int sum = 0;
                        for (int j = 0; j < 100000; ++j) {
                            sum += j;
                        }
                    } else {
                        error_count.fetch_add(1);
                    }
                } else {
                    error_count.fetch_add(1);
                }
            });
        }
        
        for (auto& thread : threads) {
            thread.join();
        }
        
        std::cout << "✅ 多线程测试结果: 成功=" << success_count.load() 
                  << ", 失败=" << error_count.load() << std::endl;
        
        TEST_ASSERT(success_count.load() > 0, "至少有线程成功绑定CPU");
        
        return true;
    }
    
    bool test_performance_benchmark() {
        std::cout << "\n=== 性能基准测试 ===" << std::endl;
        
        const size_t iterations = 1000;
        const size_t data_size = 1024;
        
        // 测试无绑定性能
        auto start_unbound = high_resolution_clock::now();
        for (size_t i = 0; i < iterations; ++i) {
            std::vector<int> data(data_size);
            std::iota(data.begin(), data.end(), 0);
            volatile int sum = std::accumulate(data.begin(), data.end(), 0);
            (void)sum;
        }
        auto end_unbound = high_resolution_clock::now();
        auto unbound_duration = duration_cast<microseconds>(end_unbound - start_unbound).count();
        
        // 测试CPU绑定性能
        std::error_code ec;
        NumaAwareCpuAllocator allocator;
        unsigned cpu = allocator.allocate_optimal_cpu(ThreadPriority::HIGH_FREQUENCY, nullptr, &ec);
        
        if (!ec) {
            bind_current_thread_to_cpu(cpu, ec);
            
            auto start_bound = high_resolution_clock::now();
            for (size_t i = 0; i < iterations; ++i) {
                std::vector<int> data(data_size);
                std::iota(data.begin(), data.end(), 0);
                volatile int sum = std::accumulate(data.begin(), data.end(), 0);
                (void)sum;
            }
            auto end_bound = high_resolution_clock::now();
            auto bound_duration = duration_cast<microseconds>(end_bound - start_bound).count();
            
            std::cout << "✅ 性能对比:" << std::endl;
            std::cout << "    未绑定: " << unbound_duration << " μs" << std::endl;
            std::cout << "    CPU绑定: " << bound_duration << " μs" << std::endl;
            
            if (bound_duration > 0) {
                double ratio = static_cast<double>(unbound_duration) / bound_duration;
                std::cout << "    性能比率: " << std::fixed << std::setprecision(2) 
                          << ratio << "x" << std::endl;
            }
            
            // 绑定后性能不应该显著变差
            TEST_ASSERT(bound_duration <= unbound_duration * 1.2, "CPU绑定后性能在合理范围内");
        }
        
        return true;
    }
    
    bool run_all_tests() {
        std::cout << "🚀 开始NUMA亲和性功能验证..." << std::endl;
        
        bool all_passed = true;
        
        all_passed &= initialize();
        all_passed &= test_basic_cpu_affinity();
        all_passed &= test_numa_topology();
        all_passed &= test_numa_cpu_allocator();
        all_passed &= test_numa_memory_allocator();
        all_passed &= test_multithread_cpu_binding();
        all_passed &= test_performance_benchmark();
        
        std::cout << "\n=== 测试总结 ===" << std::endl;
        if (all_passed) {
            std::cout << "🎉 所有测试通过！NUMA亲和性功能正常工作。" << std::endl;
        } else {
            std::cout << "❌ 部分测试失败，请检查实现。" << std::endl;
        }
        
        return all_passed;
    }
};

int main() {
    try {
        NumaAffinityValidator validator;
        bool success = validator.run_all_tests();
        return success ? 0 : 1;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ 测试异常: " << e.what() << std::endl;
        return 1;
    }
}