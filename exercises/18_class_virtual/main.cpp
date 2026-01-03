#include "../exercise.h"

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>
// 虚函数（virtual function） 是一种支持运行期多态的成员函数。
// 只要通过 基类指针 / 引用 调用虚函数，
// 实际执行的是 对象真实类型 中的实现。
// 实现多态 1. 面向接口编程 2. 框架 / 插件 / 回调机制 3. 解耦代码
struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    // override：编译期检查是否真的重写了虚函数
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    // final：禁止子类继续重写
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;
    // 初始时都是自己
    ASSERT(a.virtual_name() == 'A', MSG);
    ASSERT(b.virtual_name() == 'B', MSG);
    ASSERT(c.virtual_name() == 'C', MSG);
    // 没实现，继承上一层
    ASSERT(d.virtual_name() == 'C', MSG);
    ASSERT(a.direct_name() == 'A', MSG);
    ASSERT(b.direct_name() == 'B', MSG);
    ASSERT(c.direct_name() == 'C', MSG);
    ASSERT(d.direct_name() == 'D', MSG);

    A &rab = b;
    B &rbc = c;
    C &rcd = d;
   
    // 有多态，用子类的
    ASSERT(rab.virtual_name() == 'B', MSG);
    ASSERT(rbc.virtual_name() == 'C', MSG);
    // 有多态，没实现，用d的上一层的
    ASSERT(rcd.virtual_name() == 'C', MSG);
    // 无多态，用父类的
    ASSERT(rab.direct_name() == 'A', MSG);
    ASSERT(rbc.direct_name() == 'B', MSG);
    ASSERT(rcd.direct_name() == 'C', MSG);

    A &rac = c;
    B &rbd = d;

    // 有多态，用子类的
    ASSERT(rac.virtual_name() == 'C', MSG);
    // 有多态，没实现，用d的上一层的
    ASSERT(rbd.virtual_name() == 'C', MSG);
    // 无多态，用父类的
    ASSERT(rac.direct_name() == 'A', MSG);
    // 无多态，用父类的
    ASSERT(rbd.direct_name() == 'B', MSG);

    A &rad = d;
    
    // 有多态，没实现，用d的上一层的
    ASSERT(rad.virtual_name() == 'C', MSG);
    ASSERT(rad.direct_name() == 'A', MSG);

    return 0;
}

// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>
