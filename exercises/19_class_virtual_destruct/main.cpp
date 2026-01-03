#include "../exercise.h"

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    static int num_a;

    A() {
        ++num_a;
    }

    // delete called on non-final 'A' that has virtual functions
    // but non-virtual destructor
    virtual ~A() { // 只要类被用作多态基类，析构函数必须是 virtual
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

int A::num_a = 0;

// derived class for inheritance
struct B final : public A {
    // TODO: 正确初始化静态字段
    // non-const static data member must be initialized out of line
    // static int num_b = 0;
    // cannot assign to variable 'num_b' with const-qualified type 'const int'
    // ++num_b;
    // static const int num_b = 0;
    static int num_b;
    
    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A; // A::num_a += 1
    auto b = new B; // 先构造 A，再构造 B → A::num_a +=1, B::num_b +=1
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a; // A::num_a -= 1
    delete b; // 先构造 A，再构造 B → A::num_a +=1, B::num_b +=1
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 派生类指针可以随意转换为基类指针
                   // upcast
                   // 先构造 A，再构造 B → A::num_a +=1, B::num_b +=1 
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // downcast
    // B* bb = ab;
    // B &bb = *ab;
    B& bb = dynamic_cast<B&>(*ab);
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab;// 通过指针可以删除指向的对象，即使是多态对象
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    return 0;
}
