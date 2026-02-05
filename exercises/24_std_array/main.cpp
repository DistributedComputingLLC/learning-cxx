#include "../exercise.h"
#include <array>
#include <cstring>

// READ: std::array <https://zh.cppreference.com/w/cpp/container/array>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    {
        std::array<int, 5> arr{{1, 2, 3, 4, 5}};
        ASSERT(arr.size() == 5, "Fill in the correct value.");
        ASSERT(sizeof(arr) == sizeof(int) * 5, "Fill in the correct value.");
        int ans[]{1, 2, 3, 4, 5};
        // 第一个参数必须是 指针, 用 arr.data() 取得底层连续内存
        // 第三个参数是 字节数，用 sizeof(arr)
        // 返回值含义： 前 count 个字节 完全相等
        // 返回 < 0，在第一个不同字节处， lhs[i] < rhs[i]
        ASSERT(std::memcmp(arr.data(), ans, sizeof(arr)) == 0, "Fill in the correct values.");       
    }
    {
        // 大小在编译期固定, 即使未初始化，size() 依然是 8
        std::array<double, 8> arr;
        ASSERT(arr.size() == 8, "Fill in the correct value.");
        ASSERT(sizeof(arr) == sizeof(double) * 8, "Fill in the correct value.");
    }
    {
        std::array<char, 21> arr{"Hello, InfiniTensor!"};
        ASSERT(arr.size() == 21, "Fill in the correct value.");
        ASSERT(sizeof(arr) == sizeof(char) * 21, "Fill in the correct value.");
        ASSERT(std::strcmp(arr.data(), "Hello, InfiniTensor!") == 0, "Fill in the correct value.");
    }
    return 0;
}
