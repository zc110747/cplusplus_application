<<<<<<< HEAD
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 平凡可拷贝类型和平凡类型
//      2. 标准布局类型
//      3. 聚合类型
//      4. POD类型
//      5. 类的内存布局
//      6. 类的对齐机制
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <iostream>
#include <type_traits>

namespace TRIVIAL_COPYABLE_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass2:public MyClass {  
    };

    class MyClass3 {
    public:
        MyClass3() {
            ptr_ = new int(10);
        }
        ~MyClass3() {
            delete ptr_;
            ptr_ = nullptr;
        }
    private:
        int *ptr_{nullptr};
    };

    int process(void) 
    {
        std::cout << "============ TRIVIAL COPYABLE ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;       //true
        std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;     //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡可拷贝类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        MyClass3 obj3;

        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;      //false

        return 0;
    }
}

namespace TRIVIAL_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;
    public:
        int x_;
        double y_;
    };

    class MyClass2: public MyClass1 {
    public:
        MyClass2() = default;
    };

    int process(void)
    {
        std::cout << "============ TRIVIAL ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;                         //false
        std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;                       //true
        std::cout << "MyClass2 is trivial: " << std::is_trivial_v<MyClass2> << std::endl;                       //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        return 0;
    }    
}

namespace STANDARD_LAYOUT_TYPE
{
    class MyClass {
    public:
        MyClass() = default;

        int x_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;

    public:
        int x_;

    // 非静态成员具有不同的权限
    private:
        int y_;
    };

    class MyClass2: public MyClass {
    public:
        MyClass2() = default;

        int z_;
    };

    class MyClass3 {
    public:
        MyClass3(int x): x_(x) {
        }
        ~MyClass3() {
        }
        int x_;
    };

    int process(void)
    { 
        std::cout << "============ STANDARD LAYOUT ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is standard layout: " << std::is_standard_layout_v<MyClass> << std::endl;     //true 
        std::cout << "MyClass1 is standard layout: " << std::is_standard_layout_v<MyClass1> << std::endl;   //false
        std::cout << "MyClass2 is standard layout: " << std::is_standard_layout_v<MyClass2> << std::endl;   //false
        std::cout << "MyClass3 is standard layout: " << std::is_standard_layout_v<MyClass3> << std::endl;   //true

        MyClass3 obj3(10);
        std::cout << "obj3.x: " << obj3.x_ << std::endl;
        MyClass3 obj4(0);
        memcpy(&obj4, &obj3, sizeof(MyClass3));
        std::cout << "obj4.x: " << obj4.x_ << std::endl;

        return 0;
    }
}

namespace AGGREGATE_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    class MyStruct2 {
    private:
        int w_;
    };

    int process(void)
    {
        std::cout << "============ AGGREGATE ============" << std::endl;

        MyStruct s = {1, 2.0}; // 聚合初始化
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_aggregate_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_aggregate_v<MyStruct1> << std::endl;           //true
        std::cout << std::is_aggregate_v<MyStruct2> << std::endl;           //false

        return 0;
    }
}

namespace POD_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    template<typename T>
    constexpr auto pod_detect = std::is_trivial_v<T> && std::is_standard_layout_v<T> ;

    int process(void) 
    {
        std::cout << "============ POD ============" << std::endl;

        MyStruct s = {1, 2.0};
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_pod_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_pod_v<MyStruct1> << std::endl;           //false

        std::cout << pod_detect<MyStruct> << std::endl;               //true
        std::cout << pod_detect<MyStruct1> << std::endl;              //false

        MyStruct s2;
        memcpy(&s2, &s, sizeof(MyStruct));
        std::cout << s2.x_ << " " << s2.y_ << std::endl;

        return 0;
    }
}

namespace CLASS_MEMORY
{
    class MyClass {
    };

    class MyClass1: public MyClass {
    public:
        virtual void func1() {
        }
    };

    class MyClass2 {
        static inline int x_{1};
    };

    class MyClass3 {
    public:
        int x_{1};
    };

    int process(void) 
    {
        std::cout << "============ CLASS MEMORY ============" << std::endl;

        std::cout << sizeof(MyClass) << std::endl;      // 1
        std::cout << sizeof(MyClass1) << std::endl;     // 8
        std::cout << sizeof(MyClass2) << std::endl;     // 1
        std::cout << sizeof(MyClass3) << std::endl;     // 4

        return 0;
    }    
}

namespace ALIGNMENT
{
    struct base
    {
        char a;
        //reseved[2]
        short b;
        //reseved[4]
        double c;
    };

    template<class T> 
    struct alignof_member
    { 
        char c; 
        T member; 
    };

    template <class T>
    constexpr size_t offset_num = offsetof(alignof_member<T>, member);


    // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
    // alignas设置的值必须是2的幂值，否则无法通过编译
    // alignas设置值小于自然对齐值时，以自然对齐值为准
    struct struct_align_default
    {
        char a;
        short b;
    };

    struct alignas(1) struct_align_1
    {
        char a;
        short b;
    };

    struct alignas(4) struct_align_4
    {
        char a;
        short b;
    };

    struct alignas(8) struct_align_8
    {
        char a;
        short b;
    };

    struct struct_align_with_alignas
    {
        char a;
        alignas(4) short b;
    };

    int process(void) 
    {
        std::cout << "============== ALIGNMENT ==============" << std::endl;

        // offsetof用于获取结构体成员的偏移量
        std::cout << "struct_align_default: " << offset_num<char> << std::endl;
        std::cout << "struct_align_default: " << offset_num<int> << std::endl;
        std::cout << "struct_align_default: " << offset_num<double> << std::endl;

        // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
        std::cout << "struct_align_default: " << sizeof(struct_align_default) << std::endl;
        std::cout << "struct_align_1: " << sizeof(struct_align_1) << std::endl;
        std::cout << "struct_align_4: " << sizeof(struct_align_4) << std::endl;
        std::cout << "struct_align_8: " << sizeof(struct_align_8) << std::endl;
        std::cout << "struct_align_with_alignas: " << sizeof(struct_align_with_alignas) << std::endl;

        // alignof用于获取结构体和结构体成员的对齐值
        std::cout << "struct_align_default: " << alignof(struct_align_default) << std::endl;
        std::cout << "struct_align_default::a: " << alignof(struct_align_default::a) << std::endl;
        std::cout << "struct_align_default::b: " << alignof(struct_align_default::b) << std::endl;
        std::cout << "struct_align_1: " << alignof(struct_align_1) << std::endl;

        // alignment_of用于获取结构体和结构体成员的对齐值
        std::cout << "struct_align_1: " << std::alignment_of<struct_align_1>::value << std::endl;
        std::cout << "struct_align_4: " << std::alignment_of<struct_align_4>::value << std::endl;
        std::cout << "struct_align_8: " << std::alignment_of<struct_align_8>::value << std::endl;

        // aligned_storage_t和aligned_union_t用于分配指定对齐长度和大小的一块内存(C++23已弃用，不建议使用)
        std::aligned_storage_t<256, 8> buffer;
        std::cout << sizeof(buffer) << " " << alignof(buffer) << std::endl;                          //256 8
        std::aligned_union_t<64, int, char, double> union_b;
        std::cout << sizeof(union_b) << " " << alignof(union_b) << std::endl;                        //64 8
        return 0; 
    } 
}

int main(int argc, char *argv[])
{
    TRIVIAL_COPYABLE_TYPE::process();

    TRIVIAL_TYPE::process();

    STANDARD_LAYOUT_TYPE::process();

    AGGREGATE_TYPE::process();

    POD_TYPE::process();

    CLASS_MEMORY::process();

    ALIGNMENT::process();

    return 0;
}
=======
//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 聚合类型
//      2. 平凡可拷贝类型和平凡类型
//      3. 标准布局类型
//      4. POD类型
//      5. 类的内存布局
//      6. 类的对齐机制
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <cstring>
#include <iostream>
#include <type_traits>

namespace TRIVIAL_COPYABLE_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass2:public MyClass {  
    };

    class MyClass3 {
    public:
        MyClass3() {
            ptr_ = new int(10);
        }
        ~MyClass3() {
            delete ptr_;
            ptr_ = nullptr;
        }
    private:
        int *ptr_{nullptr};
    };

    int process(void) 
    {
        std::cout << "============ TRIVIAL COPYABLE ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass> << std::endl;       //true
        std::cout << "MyClass2 is trivially copyable: " << std::is_trivially_copyable_v<MyClass2> << std::endl;     //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡可拷贝类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        MyClass3 obj3;

        std::cout << "MyClass is trivially copyable: " << std::is_trivially_copyable_v<MyClass3> << std::endl;      //false

        return 0;
    }
}

namespace TRIVIAL_TYPE
{
    // 没有虚函数和虚基类
    class MyClass {
    public:
        MyClass(int x, double y) : x_(x), y_(y) {
        }
    public:
        int x_;
        double y_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;
    public:
        int x_;
        double y_;
    };

    class MyClass2: public MyClass1 {
    public:
        MyClass2() = default;
    };

    int process(void)
    {
        std::cout << "============ TRIVIAL ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is trivial: " << std::is_trivial_v<MyClass> << std::endl;                         //false
        std::cout << "MyClass1 is trivial: " << std::is_trivial_v<MyClass1> << std::endl;                       //true
        std::cout << "MyClass2 is trivial: " << std::is_trivial_v<MyClass2> << std::endl;                       //true

        MyClass obj1(1, 2);
        obj1.x_ = 10;

        MyClass obj2(3, 4);

        // 平凡类型，可以直接使用memcpy进行复制
        memcpy(&obj2, &obj1, sizeof(MyClass));
        std::cout << "obj2.x: " << obj2.x_ << std::endl;

        return 0;
    }    
}

namespace STANDARD_LAYOUT_TYPE
{
    class MyClass {
    public:
        MyClass() = default;

        int x_;
    };

    class MyClass1 {
    public:
        MyClass1() = default;

    public:
        int x_;

    // 非静态成员具有不同的权限
    private:
        int y_;
    };

    class MyClass2: public MyClass {
    public:
        MyClass2() = default;

        int z_;
    };

    class MyClass3 {
    public:
        MyClass3(int x): x_(x) {
        }
        ~MyClass3() {
        }
        int x_;
    };

    int process(void)
    { 
        std::cout << "============ STANDARD LAYOUT ============" << std::endl;

        std::cout << std::boolalpha;
        std::cout << "MyClass is standard layout: " << std::is_standard_layout_v<MyClass> << std::endl;     //true 
        std::cout << "MyClass1 is standard layout: " << std::is_standard_layout_v<MyClass1> << std::endl;   //false
        std::cout << "MyClass2 is standard layout: " << std::is_standard_layout_v<MyClass2> << std::endl;   //false
        std::cout << "MyClass3 is standard layout: " << std::is_standard_layout_v<MyClass3> << std::endl;   //true

        MyClass3 obj3(10);
        std::cout << "obj3.x: " << obj3.x_ << std::endl;
        MyClass3 obj4(0);
        memcpy(&obj4, &obj3, sizeof(MyClass3));
        std::cout << "obj4.x: " << obj4.x_ << std::endl;

        return 0;
    }
}

namespace AGGREGATE_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    class MyStruct2 {
    private:
        int w_;
    };

    int process(void)
    {
        std::cout << "============ AGGREGATE ============" << std::endl;

        MyStruct s = {1, 2.0}; // 聚合初始化
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_aggregate_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_aggregate_v<MyStruct1> << std::endl;           //true
        std::cout << std::is_aggregate_v<MyStruct2> << std::endl;           //false

        return 0;
    }
}

namespace POD_TYPE
{
    class MyStruct {
    public:
        int x_;
        double y_;
    };

    class MyStruct1: public MyStruct {
    public:
        int z_;
    };

    template<typename T>
    constexpr auto pod_detect = std::is_trivial_v<T> && std::is_standard_layout_v<T> ;

    int process(void) 
    {
        std::cout << "============ POD ============" << std::endl;

        MyStruct s = {1, 2.0};
        std::cout << s.x_ << " " << s.y_ << std::endl;

        std::cout << std::boolalpha;
        std::cout << std::is_pod_v<MyStruct> << std::endl;            //true

        MyStruct1 s1 = {1, 2.0, 3};
        std::cout << s1.x_ << " " << s1.y_ << " " << s1.z_ << std::endl;

        std::cout << std::is_pod_v<MyStruct1> << std::endl;           //false

        std::cout << pod_detect<MyStruct> << std::endl;               //true
        std::cout << pod_detect<MyStruct1> << std::endl;              //false

        MyStruct s2;
        memcpy(&s2, &s, sizeof(MyStruct));
        std::cout << s2.x_ << " " << s2.y_ << std::endl;

        return 0;
    }
}

namespace CLASS_MEMORY
{
    class MyClass {
    };

    class MyClass1: public MyClass {
    public:
        virtual void func1() {
        }
    };

    class MyClass2 {
        static inline int x_{1};
    };

    class MyClass3 {
    public:
        int x_{1};
    };

    int process(void) 
    {
        std::cout << "============ CLASS MEMORY ============" << std::endl;

        std::cout << sizeof(MyClass) << std::endl;      // 1
        std::cout << sizeof(MyClass1) << std::endl;     // 8
        std::cout << sizeof(MyClass2) << std::endl;     // 1
        std::cout << sizeof(MyClass3) << std::endl;     // 4

        return 0;
    }    
}

namespace ALIGNMENT
{
    struct base
    {
        char a;
        //reseved[2]
        short b;
        //reseved[4]
        double c;
    };

    template<class T> 
    struct alignof_member
    { 
        char c; 
        T member; 
    };

    template <class T>
    constexpr size_t offset_num = offsetof(alignof_member<T>, member);


    // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
    // alignas设置的值必须是2的幂值，否则无法通过编译
    // alignas设置值小于自然对齐值时，以自然对齐值为准
    struct struct_align_default
    {
        char a;
        short b;
    };

    struct alignas(1) struct_align_1
    {
        char a;
        short b;
    };

    struct alignas(4) struct_align_4
    {
        char a;
        short b;
    };

    struct alignas(8) struct_align_8
    {
        char a;
        short b;
    };

    struct struct_align_with_alignas
    {
        char a;
        alignas(4) short b;
    };

    int process(void) 
    {
        std::cout << "============== ALIGNMENT ==============" << std::endl;

        // offsetof用于获取结构体成员的偏移量
        std::cout << "struct_align_default: " << offset_num<char> << std::endl;
        std::cout << "struct_align_default: " << offset_num<int> << std::endl;
        std::cout << "struct_align_default: " << offset_num<double> << std::endl;

        // alignas用于设置结构体的对齐值(alignas(1)、alignas(2)、alignas(4)、alignas(8)等，)
        std::cout << "struct_align_default: " << sizeof(struct_align_default) << std::endl;
        std::cout << "struct_align_1: " << sizeof(struct_align_1) << std::endl;
        std::cout << "struct_align_4: " << sizeof(struct_align_4) << std::endl;
        std::cout << "struct_align_8: " << sizeof(struct_align_8) << std::endl;
        std::cout << "struct_align_with_alignas: " << sizeof(struct_align_with_alignas) << std::endl;

        // alignof用于获取结构体和结构体成员的对齐值
        std::cout << "struct_align_default: " << alignof(struct_align_default) << std::endl;
        std::cout << "struct_align_default::a: " << alignof(struct_align_default::a) << std::endl;
        std::cout << "struct_align_default::b: " << alignof(struct_align_default::b) << std::endl;
        std::cout << "struct_align_1: " << alignof(struct_align_1) << std::endl;

        // alignment_of用于获取结构体和结构体成员的对齐值
        std::cout << "struct_align_1: " << std::alignment_of<struct_align_1>::value << std::endl;
        std::cout << "struct_align_4: " << std::alignment_of<struct_align_4>::value << std::endl;
        std::cout << "struct_align_8: " << std::alignment_of<struct_align_8>::value << std::endl;

        // aligned_storage_t和aligned_union_t用于分配指定对齐长度和大小的一块内存(C++23已弃用，不建议使用)
        std::aligned_storage_t<256, 8> buffer;
        std::cout << sizeof(buffer) << " " << alignof(buffer) << std::endl;                          //256 8
        std::aligned_union_t<64, int, char, double> union_b;
        std::cout << sizeof(union_b) << " " << alignof(union_b) << std::endl;                        //64 8
        return 0; 
    } 
}

int main(int argc, char *argv[])
{
    TRIVIAL_COPYABLE_TYPE::process();

    TRIVIAL_TYPE::process();

    STANDARD_LAYOUT_TYPE::process();

    AGGREGATE_TYPE::process();

    POD_TYPE::process();

    CLASS_MEMORY::process();

    ALIGNMENT::process();

    return 0;
}
>>>>>>> 171775b3443edbb693d4aa0be5b9466005143b33
