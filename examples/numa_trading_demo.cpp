// 高频交易NUMA优化示例
#include "affinity.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>

using namespace api;
using namespace std::chrono;

// 模拟市场数据结构
struct MarketData {
    uint64_t timestamp;
    uint32_t symbol_id;
    double price;
    uint64_t volume;
    
    MarketData() : timestamp(0), symbol_id(0), price(0.0), volume(0) {}
    MarketData(uint64_t ts, uint32_t sid, double p, uint64_t v) 
        : timestamp(ts), symbol_id(sid), price(p), volume(v) {}
};

// 模拟交易订单
struct TradeOrder {
    uint64_t order_id;
    uint32_t symbol_id;
    double price;
    uint64_t quantity;
    char side; // 'B' for buy, 'S' for sell
    
    TradeOrder() : order_id(0), symbol_id(0), price(0.0), quantity(0), side('B') {}
};

// NUMA感知的高频交易引擎示例
class HighFrequencyTradingEngine {
private:
    NumaAwareCpuAllocator cpu_allocator;
    NumaTopology topology;
    
    // 使用NUMA感知分配器的数据结构
    std::vector<MarketData, NumaAwareAllocator<MarketData>> market_data_buffer;
    std::vector<TradeOrder, NumaAwareAllocator<TradeOrder>> order_buffer;
    
public:
    HighFrequencyTradingEngine() : cpu_allocator(CpuAllocationStrategy::NUMA_LOCAL) {
        std::error_code ec;
        topology = get_numa_topology(ec);
        
        if (!ec) {
            std::cout << "=== NUMA拓扑信息 ===" << std::endl;
            std::cout << "NUMA节点数: " << topology.node_count << std::endl;
            std::cout << "NUMA支持: " << (topology.is_numa_available ? "是" : "否") << std::endl;
            
            for (unsigned node = 0; node < topology.node_count; ++node) {
                std::cout << "节点 " << node << ": " 
                         << topology.node_cpus[node].size() << " CPUs, "
                         << topology.node_memory_sizes[node] << " MB内存" << std::endl;
                std::cout << "  CPUs: ";
                for (unsigned cpu : topology.node_cpus[node]) {
                    std::cout << cpu << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    
    // 启动市场数据接收线程（关键路径）
    void launch_market_data_thread() {
        std::thread md_thread([this]() {
            std::cout << "启动市场数据线程..." << std::endl;
            
            // 为关键线程分配隔离CPU
            std::error_code ec;
            unsigned cpu = cpu_allocator.allocate_isolated_cpu(ec);
            if (!ec) {
                bind_current_thread_to_cpu(cpu, ec);
                std::cout << "市场数据线程绑定到隔离CPU: " << cpu 
                         << " (NUMA节点: " << topology.cpu_to_node[cpu] << ")" << std::endl;
            }
            
            // 分配NUMA本地内存
            NumaAwareAllocator<MarketData> allocator;
            const size_t buffer_size = 10000;
            MarketData* local_buffer = allocator.allocate_local(buffer_size, ec);
            
            if (!ec) {
                std::cout << "分配了 " << buffer_size << " 个MarketData的本地内存" << std::endl;
                
                // 模拟高频数据处理
                auto start = high_resolution_clock::now();
                for (size_t i = 0; i < buffer_size; ++i) {
                    // 模拟市场数据写入
                    local_buffer[i] = MarketData(
                        duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count(),
                        i % 1000,  // symbol_id
                        100.0 + (i % 100) * 0.01,  // price
                        1000 + i % 5000  // volume
                    );
                    
                    // 模拟一些计算（价格变化检测等）
                    if (i > 0 && local_buffer[i].price != local_buffer[i-1].price) {
                        // 价格变化逻辑
                    }
                }
                auto end = high_resolution_clock::now();
                
                auto duration = duration_cast<nanoseconds>(end - start).count();
                std::cout << "处理 " << buffer_size << " 条市场数据耗时: " 
                         << duration << " ns (平均 " << duration / buffer_size << " ns/条)" << std::endl;
                
                allocator.deallocate_numa(local_buffer, buffer_size, ec);
            }
            
            std::cout << "市场数据线程完成" << std::endl;
        });
        
        md_thread.join();
    }
    
    // 启动交易执行线程
    void launch_trading_thread() {
        std::thread trading_thread([this]() {
            std::cout << "\n启动交易执行线程..." << std::endl;
            
            // 为交易线程选择最优CPU（考虑负载均衡）
            std::error_code ec;
            unsigned cpu = cpu_allocator.allocate_optimal_cpu(ThreadPriority::HIGH_FREQUENCY, nullptr, &ec);
            if (!ec) {
                bind_current_thread_to_cpu(cpu, ec);
                std::cout << "交易线程绑定到CPU: " << cpu 
                         << " (NUMA节点: " << topology.cpu_to_node[cpu] << ")" << std::endl;
            }
            
            // 使用NUMA感知分配器
            NumaAwareAllocator<TradeOrder> allocator;
            const size_t order_count = 5000;
            TradeOrder* orders = allocator.allocate_local(order_count, ec);
            
            if (!ec) {
                // 模拟订单处理
                auto start = high_resolution_clock::now();
                for (size_t i = 0; i < order_count; ++i) {
                    orders[i].order_id = i + 1;
                    orders[i].symbol_id = i % 500;
                    orders[i].price = 100.0 + (i % 50) * 0.05;
                    orders[i].quantity = 100 + i % 1000;
                    orders[i].side = (i % 2) ? 'B' : 'S';
                    
                    // 模拟订单验证和风控
                    if (orders[i].price > 0 && orders[i].quantity > 0) {
                        // 订单有效
                    }
                }
                auto end = high_resolution_clock::now();
                
                auto duration = duration_cast<nanoseconds>(end - start).count();
                std::cout << "处理 " << order_count << " 个订单耗时: " 
                         << duration << " ns (平均 " << duration / order_count << " ns/订单)" << std::endl;
                
                allocator.deallocate_numa(orders, order_count, ec);
            }
            
            std::cout << "交易执行线程完成" << std::endl;
        });
        
        trading_thread.join();
    }
    
    // 显示分配统计信息
    void show_allocation_stats() {
        auto stats = cpu_allocator.get_allocation_stats();
        
        std::cout << "\n=== CPU分配统计 ===" << std::endl;
        std::cout << "总分配次数: " << stats.total_allocations << std::endl;
        std::cout << "隔离分配次数: " << stats.isolated_allocations << std::endl;
        std::cout << "各节点分配次数:" << std::endl;
        
        for (size_t i = 0; i < stats.node_allocations.size(); ++i) {
            std::cout << "  节点 " << i << ": " << stats.node_allocations[i] << " 次" << std::endl;
        }
    }
    
    // 内存访问性能测试
    void benchmark_memory_access() {
        std::cout << "\n=== 内存访问性能测试 ===" << std::endl;
        
        const size_t test_size = 1024 * 1024; // 1M个double
        const size_t iterations = 100;
        
        std::error_code ec;
        NumaAwareAllocator<double> allocator;
        
        // 测试本地NUMA节点访问
        double* local_memory = allocator.allocate_local(test_size, ec);
        if (!ec) {
            auto start = high_resolution_clock::now();
            for (size_t iter = 0; iter < iterations; ++iter) {
                for (size_t i = 0; i < test_size; ++i) {
                    local_memory[i] = static_cast<double>(i) * 1.5;
                }
            }
            auto end = high_resolution_clock::now();
            
            auto local_duration = duration_cast<nanoseconds>(end - start).count();
            double local_ns_per_op = static_cast<double>(local_duration) / (iterations * test_size);
            
            std::cout << "本地内存访问: " << std::fixed << std::setprecision(2) 
                     << local_ns_per_op << " ns/操作" << std::endl;
            
            allocator.deallocate_numa(local_memory, test_size, ec);
        }
        
        // 如果有多个NUMA节点，测试跨节点访问
        if (topology.node_count > 1) {
            unsigned current_node = get_current_numa_node(ec);
            unsigned remote_node = (current_node + 1) % topology.node_count;
            
            double* remote_memory = allocator.allocate_on_numa_node(test_size, remote_node, ec);
            if (!ec) {
                auto start = high_resolution_clock::now();
                for (size_t iter = 0; iter < iterations; ++iter) {
                    for (size_t i = 0; i < test_size; ++i) {
                        remote_memory[i] = static_cast<double>(i) * 1.5;
                    }
                }
                auto end = high_resolution_clock::now();
                
                auto remote_duration = duration_cast<nanoseconds>(end - start).count();
                double remote_ns_per_op = static_cast<double>(remote_duration) / (iterations * test_size);
                
                std::cout << "远程内存访问: " << std::fixed << std::setprecision(2) 
                         << remote_ns_per_op << " ns/操作" << std::endl;
                
                allocator.deallocate_numa(remote_memory, test_size, ec);
            }
        }
    }
};

int main() {
    try {
        std::cout << "=== 高频交易NUMA优化演示 ===" << std::endl;
        
        HighFrequencyTradingEngine engine;
        
        // 启动关键线程
        engine.launch_market_data_thread();
        engine.launch_trading_thread();
        
        // 显示统计信息
        engine.show_allocation_stats();
        
        // 性能测试
        engine.benchmark_memory_access();
        
        std::cout << "\n演示完成!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}