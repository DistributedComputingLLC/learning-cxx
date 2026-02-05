#include "../exercise.h"

// READ: Trivial type <https://learn.microsoft.com/zh-cn/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170>

struct FibonacciCache {
    unsigned long long cache[16];
    int cached; // cached 表示：cache.cache 中「已经被正确计算并可直接使用的 Fibonacci 项的数量」
                // 下一个“尚未计算”的下标
};

// TODO: 实现正确的缓存优化斐波那契计算
// FibonacciCache &cache is pass by ref, other than pass by value, no copy
static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    // 边界保护
    if (i < 0 || i >= 16) {
        return 0;
    }

    // 若尚未计算到 i，则继续计算
    for (; cache.cached <= i; ++cache.cached) {
        // 直接作用在原对象上, 多次调用能共享缓存
        cache.cache[cache.cached] = cache.cache[cache.cached - 1] + cache.cache[cache.cached - 2];
    }
    
    // // 若尚未计算到 i，则继续计算
    // for (int j = cache.cached; j <= i; ++j) {
    //     // 直接作用在原对象上, 多次调用能共享缓存
    //     cache.cache[j] = cache.cache[j - 1] + cache.cache[j - 2];
    // }

    // // 更新缓存范围
    // if (cache.cached <= i) {
    //     // 直接作用在原对象上, 多次调用能共享缓存
    //     cache.cached = i + 1;
    // }

    return cache.cache[i];
}

int main(int argc, char **argv) {
    // TODO: 初始化缓存结构体，使计算正确
    // NOTICE: C/C++ 中，读取未初始化的变量（包括结构体变量）是未定义行为
    // READ: 初始化的各种写法 <https://zh.cppreference.com/w/cpp/language/initialization>
    FibonacciCache fib = {{0, 1}, 2};

    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}
