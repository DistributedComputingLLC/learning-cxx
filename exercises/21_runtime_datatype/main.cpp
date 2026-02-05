#include "../exercise.h"
#include <cmath>

enum class DataType {
    Float,
    Double,
};

/// @brief Tagged union 即标签化联合体，是联合体的一种常见应用。
///        Rust enum 在实现上就是标签化联合体。
struct TaggedUnion {
    DataType type; // tag = 当前 union 中“哪个字段是有效的”
    // NOTICE: struct/union 可以相互任意嵌套。
    union {  // 所有成员共享同一块内存
             // 大小 = 最大成员的大小（这里是 double）
        float f;
        double d;
    };
};

// TODO: 将这个函数模板化用于 sigmoid_dyn
// double 会发生：隐式转换, 精度损失
// float sigmoid(float x) {
//     return 1 / (1 + std::exp(-x));
// }
template <typename T>
T sigmoid(T x) {
    // 整数 → 浮点混合运算
    // T(1) direct-initialization
    // float(1) double(1) when T is floar / double
    return T(1) / (T(1) + std::exp(-x));
}
// 没有虚函数, 编译器完全可见, 吞吐量, no 多态
// ex: softmax, convolution, layer norm

TaggedUnion sigmoid_dyn(TaggedUnion x) {
    TaggedUnion ans{x.type};
    // TODO: 根据 type 调用 sigmoid
    switch (x.type) { // 编译器可做 exhaustiveness 检查（警告未覆盖分支）
        case DataType::Float:
            ans.f = sigmoid(x.f);
            break;
        case DataType::Double:
            ans.d = sigmoid(x.d);
            break;
    }
    return ans;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    TaggedUnion xf{DataType::Float};
    xf.f = 5.f;
    auto yf = sigmoid_dyn(xf);
    ASSERT(yf.type == DataType::Float, "type mismatch");
    ASSERT(yf.f == 1 / (1 + std::exp(-5.f)), "sigmoid float");

    TaggedUnion xd{DataType::Double};
    xd.d = 5.0;
    auto yd = sigmoid_dyn(xd);
    ASSERT(yd.type == DataType::Double, "type mismatch");
    ASSERT(yd.d == 1 / (1 + std::exp(-5.0)), "sigmoid double");
    return 0;
}
