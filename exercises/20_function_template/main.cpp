#include "../exercise.h"
#include <cmath>

// READ: 函数模板 <https://zh.cppreference.com/w/cpp/language/function_template>
// TODO: 将这个函数模板化
// int plus(int a, int b) {
//     return a + b;
// }
// 函数模板：支持任意可相加的类型
// 编译期类型推导,编译器会自动生成多个函数版本 (blueprint)
// zero-cost abstraction
template <typename T>
T plus(T a, T b) {
    return a + b;
}

int main(int argc, char **argv) {
    ASSERT(plus(1, 2) == 3, "Plus two int");
    ASSERT(plus(1u, 2u) == 3u, "Plus two unsigned int");

    // THINK: 浮点数何时可以判断 ==？何时必须判断差值？
    // 在二进制中可以被精确表示 then 可以判断 ==
    ASSERT(plus(1.25f, 2.5f) == 3.75f, "Plus two float");
    ASSERT(plus(1.25, 2.5) == 3.75, "Plus two double");
    // TODO: 修改判断条件使测试通过
    // ASSERT(plus(0.1, 0.2) == 0.3, "How to make this pass?");
    // 使用“误差范围”判断浮点数相等
    // 浮点数是 二进制近似值, 是 IEEE-754 标准
    // 选择 epsilon:
    // float 1e-6
    // double 1e-9
    // long double 1e-12
    ASSERT(std::fabs(plus(0.1, 0.2) - 0.3) < 1e-9,
           "How to make this pass?");

    return 0;
}
