#include "../exercise.h"
#include <cstring>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T> // Class Template, 编译期多态
struct Tensor4D {
    // shape 是数组，不是对象。数组在 C++ 里不能被整体赋值。
    unsigned int shape[4];
    T *data; // Resource Acquisition Is Initialization
    
    // same as Tensor4D(unsigned int const* shape_, ...)
    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        // std::memcpy(shape, shape_, 4 * sizeof(unsigned int));

        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    // default copy cstr 两个对象指向同一块内存
    // prevent double free during dstr
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持“单向广播”。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    // Tensor4D &operator+=(Tensor4D const &others) {
    //     // TODO: 实现单向广播的加法
    //     // 计算 this 和 others 的 stride
    //     unsigned int stride_this[4];
    //     unsigned int stride_other[4];

    //     stride_this[3] = 1;
    //     stride_other[3] = 1;
    //     for (int i = 2; i >= 0; --i) {
    //         stride_this[i] = stride_this[i + 1] * shape[i + 1];
    //         stride_other[i] = stride_other[i + 1] * others.shape[i + 1];
    //         // std::cout << "i:" << i << std::endl;
    //         // std::cout << stride_this[i] << std::endl;
    //         // std::cout << stride_other[i] << std::endl;
    //     }
    //     // stride_this:  [24, 12, 4, 1]
    //     // stride_other: [24, 12, 4, 1]

    //     unsigned int total = 1;
    //     for (int i = 0; i < 4; ++i) {
    //         total *= shape[i];
    //     }
    //     // total = 24
        
    //     // N-d to 1d via stride
    //     for (unsigned int idx = 0; idx < total; ++idx) {
    //         unsigned int rem = idx;
    //         unsigned int offset_other = 0;

    //         for (int d = 0; d < 4; ++d) {
    //             unsigned int coord = rem / stride_this[d];
    //             rem %= stride_this[d];
    //             // std::cout << "d: " << d << "coord: " << coord << "rem: " << rem << std::endl;

    //             unsigned int other_coord =
    //                 (others.shape[d] == 1) ? 0 : coord;

    //             offset_other += other_coord * stride_other[d];
    //             // std::cout << "d: " << d << "other_coord: " << other_coord << "offset_other: " << offset_other << std::endl;
    //         }

    //         data[idx] += others.data[offset_other];
    //         std::cout << "idx: " << idx << " offset_other: " << offset_other << std::endl;

    //     }

    //     return *this;
    // }
    Tensor4D &operator+=(Tensor4D const &others) {
        unsigned int stride_this[4];
        unsigned int stride_other[4];

        stride_this[3] = 1;
        for (int i = 2; i >= 0; --i) {
            stride_this[i] = stride_this[i + 1] * shape[i + 1];
        }

        stride_other[3] = 1;
        for (int i = 2; i >= 0; --i) {
            stride_other[i] = stride_other[i + 1] * others.shape[i + 1];
        }

        for (int i = 0; i < 4; ++i) {
            if (others.shape[i] == 1) {
                stride_other[i] = 0;   // broadcast
            }
        }

        unsigned int total = 1;
        for (int i = 0; i < 4; ++i) {
            total *= shape[i];
        }

        for (unsigned int idx = 0; idx < total; ++idx) {
            unsigned int rem = idx;
            unsigned int offset_this = 0;
            unsigned int offset_other = 0;

            for (int d = 0; d < 4; ++d) {
                unsigned int coord = rem / stride_this[d];
                rem %= stride_this[d];

                offset_this  += coord * stride_this[d];
                offset_other += coord * stride_other[d];  // broadcast 在这里生效
            }

            data[offset_this] += others.data[offset_other];
        }

        return *this;
    }

};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // std::cout << sizeof(data) << std::endl;
        // std::cout << sizeof(*data) << std::endl;

        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
    }
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
    }
}
