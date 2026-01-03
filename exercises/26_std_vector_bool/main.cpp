#include "../exercise.h"
#include <vector>

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector_bool>
// READ: 模板特化 <https://zh.cppreference.com/w/cpp/language/template_specialization>

// 不存 bool, 按 bit 压缩存储, 一个 bool ≈ 1 bit
// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    std::vector<bool> vec(100, true);// TODO: 正确调用构造函数
    ASSERT(vec[0], "Make this assertion pass.");
    ASSERT(vec[99], "Make this assertion pass.");
    ASSERT(vec.size() == 100, "Make this assertion pass.");
    // NOTICE: 平台相关！注意 CI:Ubuntu 上的值。
    std::cout << "sizeof(std::vector<bool>) = " << sizeof(std::vector<bool>) << std::endl;
    // Ubuntu 64-bit + libstdc++ , vector<bool> size is 40, 额外保存：
    // bitset 数据指针
    // size, capacity
    // bit 操作相关结构
    // 所以 比普通 std::vector<T> 大
    ASSERT(sizeof(vec) == 24, "Fill in the correct value.");
    {
        vec[20] = false;
        ASSERT(!vec[20], "Fill in `vec[20]` or `!vec[20]`.");
    }
    {
        vec.push_back(false);
        ASSERT(vec.size() == 101, "Fill in the correct value.");
        ASSERT(!vec[100], "Fill in `vec[100]` or `!vec[100]`.");
    }
    {
        // ref is std::vector<bool>::reference
        // 它是一个 代理对象（proxy），内部保存：
        // 指向 bit 的地址
        // bit 的偏移量
        // reference 重载了
        auto ref = vec[30];
        ASSERT(ref, "Fill in `ref` or `!ref`");
        ref = false;
        ASSERT(!ref, "Fill in `ref` or `!ref`");
        // THINK: WHAT and WHY?
        ASSERT(!vec[30], "Fill in `vec[30]` or `!vec[30]`.");
    }
    return 0;
}
