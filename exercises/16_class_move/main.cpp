#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int capacity)
        : cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // TODO: 实现移动构造器
    // DynFibonacci(DynFibonacci &&) noexcept = delete;
    // && 只接受可被掏空的对象（右值）
    // noexcept 这个函数绝对不会抛异常，可以用移动而不是拷贝
    DynFibonacci(DynFibonacci &&other) noexcept
        : cache(other.cache), cached(other.cached) {
        // 被移动的对象不能释放资源，因为资源已经“转移”给新对象了
        other.cache = nullptr;
        other.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    // DynFibonacci &operator=(DynFibonacci &&) noexcept = delete;
    // DynFibonacci operator=(DynFibonacci&&);
    // 问题非常严重：1. 返回临时对象 2.需要调用拷贝 / 移动构造 
    // 3. 可能无限递归 4. 完全不符合赋值语义
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if (this == &other)
            return *this;
        // 防止内存泄漏
        delete[] cache;

        cache = other.cache;
        cached = other.cached;

        other.cache = nullptr;
        other.cached = 0;

        return *this;
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }
    
    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {  
        for (; cached <= i; ++cached) {  
            cache[cached] = cache[cached - 1] + cache[cached - 2];  
        }  
        return cache[i];  
    }

    // NOTICE: 不要修改这个方法
    size_t operator[](int i) const {
        ASSERT(i <= cached, "i out of range");
        return cache[i];
    }

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    ASSERT(!fib.is_alive(), "Object moved");
    ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");

    return 0;
}
