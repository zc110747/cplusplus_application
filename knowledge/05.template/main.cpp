//////////////////////////////////////////////////////////////////////////////
//  (c) copyright 2023-by ZC Inc.  
//  All Rights Reserved
//
//  Name:
//      main.cpp
//
//  Purpose:
//      1. 模板类型
//      2. 模板用法
//      3. 模板规则
//
// Author:
//      @zc
//
// Revision History:
//      Version V1.0b1 Create.
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <type_traits>
#include <vector>
#include <utility>
#include <tuple>

// 模板类型基础
namespace TYPE
{
// 函数模板，类型模板
template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

// 类模板
template <class T>
class Stack {
private:
    std::vector<T> elements;
public:
    Stack() = default;

    void push(T const& a) {
        elements.push_back(a);
    }
    void pop(void) {
        elements.pop_back();
    }
    void show (void) {
        for(auto val:elements) {
            std::cout<<val<<" ";
        }
        std::cout<<std::endl; 
    }
    bool empty(void) const {
        return elements.empty();
    }
};

// 变量模板
template <typename T>
constexpr T pi = T(3.1415926535897932385);

// using声明别名模板
template <typename T>
using Vec = std::vector<T>;

// 非类型模板
template <typename T, int N>
class Array {
private:
    T elems[N];
public:
    constexpr T& operator[](std::size_t index) {
        return elems[index];
    }
    constexpr std::size_t size() const {
        return N;
    }
    Array(std::initializer_list<T> list){
        if(list.size() > N) {
            throw std::length_error("Array::Array");
        }
        std::copy(list.begin(), list.end(), elems);
    }  
};

template <const int &N>
class ArrayRef {
public:
    void print() {
        std::cout<<"Value: "<<N<<std::endl;
    }
};

void test(void)
{
    std::cout<<"=== TYPE ===\n";
    std::cout << "max(1, 2) = " << max(1, 2) << std::endl;
    std::cout << "max(1.0, 2.0) = " << max<float>(1.0, 2.0) << std::endl;

    Stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.show();

    std::cout<<"pi:"<<pi<double><<std::endl;

    Vec<int> vec{1, 2, 3};
    for(auto val:vec) {
        std::cout<<val<<" ";
    }
    std::cout<<std::endl;

    Array<int, 5> arr{1, 2, 3, 4, 5};
    for(int i = 0; i < arr.size(); i++) {
        std::cout<<arr[i]<<" ";
    }
    std::cout<<std::endl;

    const static int value = 10;
    ArrayRef<value> arrRef;
    arrRef.print();             //10
}
}

// 模板应用
namespace APPLICATION
{
// 模板作为模板的参数，需要使用class指定
template <template <typename, typename> class Container, typename T>
class Stack {
private:
    Container<T, std::allocator<T>> elements;

public:
    void push(const T& value) {
        elements.push_back(value);
    }

    void pop() {
        if (!elements.empty()) {
            elements.pop_back();
        }
    }

    T top() const {
        return elements.back();
    }

    bool empty() const {
        return elements.empty();
    }

    void show() {
        for (const auto& element : elements) {
            std::cout << element << " "; 
        }
        std::cout<<std::endl;
    }
};

//可变参数模板函数
template<typename... Args>
void print(Args... args) {
    std::cout << "Number of arguments: " << sizeof...(Args) << std::endl;
    int dummy[] = {0, (std::cout << args << " ", 0)...};
    std::cout<<std::endl;
    
    static_cast<void>(dummy); // 避免编译器警告
}


template<typename T>
T sum(T arg) {
    return arg;
}

template<typename First, typename... Rest>
First sum(First first, Rest... rest) {
    return first + sum(rest...);
}

//默认模板参数
template<class T, class U = int>
void f(T t, U u= 0)
{
    std::cout<<"t= "<<t<<" u= "<<u<<std::endl;
}

// 模板特化
// 通用类模板
template <typename T1 = int, typename T2 = int>
class Pair {
public:
    T1 first;
    T2 second;

    Pair(const T1& f, const T2& s) : first(f), second(s) {}

    void print() const {
        std::cout << "General template: (" << first << ", " << second << ")" << std::endl;
    }
};

// 偏特化类模板 
// for std::string and std::vector
template <typename T>
class Pair<std::string, std::vector<T>> {
public:
    std::string first;
    std::vector<T> second;

    Pair(const std::string& f, const std::vector<T>& s) : first(f), second(s) {}

    void print() const {
        std::cout << "Partially specialized template for std::string and std::vector: (" << first << ", ";
        for (const auto& elem : second) {
            std::cout << elem << " ";
        }
        std::cout << ")" << std::endl;
    }
};

// 特化类模板 for std::string and int
template <>
class Pair<std::string, int> {
public:
    std::string first;
    int second;

    Pair(const std::string& f, const int& s) : first(f), second(s) {}

    void print() const {
        std::cout << "Specialized template for std::string and int: (" << first << ", " << second << ")" << std::endl;
    }
};

template<typename T>
T get_value() {
    return T();
}

template<typename T>
auto add(T a, T b) -> decltype(a + b) {
    return a + b;
}

template<typename T>
class Container {
public:
    Container(T val1, T val2) : data1_(val1), data2_(val2) {}

    T getData(int index = 0) const {
        if (index == 0) {
            return data1_; 
        }
        return data2_;
    }

private:
    T data1_;
    T data2_;
};

void test(void)
{
    std::cout<<"=== APPLICATION ===\n";
    Stack<std::vector, int> s1;
    s1.push(1);
    s1.show();

    // 可变参数模板
    print(1, 2, 3, 4, 5);
    print("hello", "world");

    std::cout<<sum(1, 2, 3, 4, 5)<<std::endl;

    Pair<int, double> p1(1, 2.5);
    p1.print(); // 使用通用模板

    Pair p2(1, 2);
    p2.print(); // 使用通用模板

    Pair<std::string, int> p3("Hello", 42);
    p3.print(); // 使用特化模板
    
    Pair<std::string, std::vector<int>> p4("Numbers", {1, 2, 3, 4, 5});
    p4.print(); // 使用偏特化模板

    //默认模板参数
    f(1, 2);
    f(1);
    f("c", 1);

    //模板的自动推断和显示指定
    //模板函数调用时，参数无法提供足够信息，需要指定
    int value = get_value<int>();
    std::cout << "Value: " << value << std::endl;

    auto result = add(1, 2);
    std::cout << "Result: " << result << std::endl;
    
    //模板函数类型推导存在歧义
    result = add<int>(1, 2.5);
    std::cout << "Result: " << result << std::endl;

    //模板类类型推导存在歧义
    Container<int> Container(2, 3.2);
    std::cout << "Data: " << Container.getData() << std::endl;
}
}

// 模板规则
namespace RULE
{
// ADL查找规则
namespace MyNamespace {
    class MyString {
    public:
        MyString(const char* str) : data(str) {}
        const char* data;
    };

    void print(const MyString& str) {
        std::cout << str.data << std::endl;
    }
}

// auto声明非类型模板参数(一元左折叠)
template <auto... seq>
void print_sequence() {
    ((std::cout << seq << " "), ...);
    std::cout << std::endl;
}

// 折叠表达式
// 一元左折叠
template <typename... Args>
auto left_fold(Args... args) {
    return (... + args);
}

// 一元右折叠
template <typename... Args>
auto right_fold(Args... args) {
    return (args + ...);
}

// 二元左折叠
template <typename... Args>
auto left_fold_with_init(int init, Args... args) {
    return (init + ... + args);
}

// 二元右折叠
template <typename... Args>
auto right_fold_with_init(int init, Args... args) {
    return (args + ... + init);
}


// 用户自定义推导指引
template <typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    Pair(T1 f, T2 s) : first(f), second(s) {}
    void show() {
        std::cout<<"first:"<<first<<", second:"<<second<<std::endl;
    }
};

// 推导指引
template <typename T1, typename T2>
Pair(T1, T2) -> Pair<T1, T2>;

// 主模板
template <typename T, typename = void>
struct has_output_operator : std::false_type {};

// 特化模板，当T支持operator<<时匹配
template <typename T>
struct has_output_operator<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : std::true_type {};

// 使用SFINAE的函数模板
template <typename T>
typename std::enable_if<has_output_operator<T>::value, void>::type
print(T value) {
    std::cout << "Value: " << value << std::endl;
}

struct Test{
    typedef int foo;
};

template<typename T>
void f(T) {
    std::cout << "T" << std::endl;
}

template<typename T>
void f(typename T::foo) {
    std::cout << "T:foo" << std::endl;
}

// 备用函数模板，当T不支持operator<<时调用
template <typename T>
typename std::enable_if<!has_output_operator<T>::value, void>::type
print(T value) {
    std::cout << "Type does not support output operator" << std::endl;
}

void test(void)
{
    std::cout<<"=== RULE ===\n";
    MyNamespace::MyString str("Hello, world!");
    print(str); //使用ADL规则查找print函数

    // auto声明非类型模板参数
    print_sequence<1, 2, 3, 4, 5>();

    auto result = left_fold(1, 2, 3, 4, 5);
    std::cout << "Left fold result: " << result << std::endl;
    result = right_fold(1, 2, 3, 4, 5);
    std::cout << "Right fold result: " << result << std::endl;
    result = left_fold_with_init(10, 1, 2, 3, 4, 5);
    std::cout << "Left fold with init result: " << result << std::endl;
    result = right_fold_with_init(10, 1, 2, 3, 4, 5);
    std::cout << "Right fold with init result: " << result << std::endl;

    // SFINAE
    f<Test>(10);
    f<int>(10);

    Pair p1(1, 2.5);
    p1.show();

    auto p2 = Pair("hello", 42);
    p2.show();

    print(42); // 调用第一个print，因为int支持operator<<
    print(std::string("Hello")); // 调用第一个print，因为std::string支持operator<<
    print(std::vector<int>{1, 2, 3}); // 调用第二个print，因为std::vector<int>不支持operator<<
}
}

// 模板元编程
namespace MEATPROGRAMING
{
// 模板元编程
template<bool cond, typename First, typename Second>
struct IF;

template<typename First, typename Second>
struct IF<true, First, Second>
{
    typedef First result;
};

template<typename First, typename Second>
struct IF<false, First, Second>
{
    typedef Second result;
};

//add
template<int First, int... rest>
struct add_all_;

template <int First>
struct add_all_<First>:std::integral_constant<int, First>
{
};

template <int First, int Second, int... Rest>
struct add_all_<First, Second, Rest...>
    :std::integral_constant<int, add_all_<First>::value + add_all_<Second, Rest...>::value>
{
};

//sub(第一个值减去后面之和)
template<int First, int... Rest>
struct sub_all_;

template <int arg>
struct sub_all_<arg>:std::integral_constant<int, arg>
{
};

template <int First, int Second, int... Rest>
struct sub_all_<First, Second, Rest...>
    :std::integral_constant<int, sub_all_<First>::value - add_all_<Second, Rest...>::value>
{
};

template<bool cond, int arg, int... rest>
struct add_sub_all
{
    static const int res_value = 
        IF<cond, add_all_<arg, rest...>, sub_all_<arg, rest...>>::result::value;
};

template<int N>
struct AbsoluteValue {
    static constexpr int value = (N < 0) ? -N : N;
};

template<int N>
constexpr int is_negative() {
    return (N < 0)? 1 : 0;
}

template <char... Char> struct string_holder {
  static constexpr char value[] = {Char...};
  static constexpr std::size_t len = sizeof...(Char);
  constexpr operator std::string_view() const { return {value, len}; }
};

template <int N, char... Char> 
constexpr auto int_to_string() 
{
    if constexpr (N < 0) {
        return int_to_string<AbsoluteValue<N>::value, Char...>();
    } else if constexpr (N < 10) {
        return string_holder<is_negative<N>()?'-':'0', N + '0', Char...>{};
    } else {
        return int_to_string<N / 10, (N % 10) + '0', Char...>();
    }
}

constexpr std::string_view sv = int_to_string<99999>();
constexpr auto sv2 = AbsoluteValue<-10>::value;

// 打印整数序列
template<typename T, T... Ints>
void print_sequence(const std::integer_sequence<T, Ints...>&) {
    ((std::cout << Ints << " "), ...);
    std::cout << std::endl;
}

// 使用 std::index_sequence 展开参数包
template<typename Tuple, std::size_t... Is>
void print_tuple(const Tuple& t, std::index_sequence<Is...>) {
    ((std::cout << (Is == 0? "" : ", ") << std::get<Is>(t)), ...);
    std::cout << std::endl;
}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& t) {
    print_tuple(t, std::make_index_sequence<sizeof...(Args)>{});
}

void test(void)
{
    std::cout<<"=== MEATPROGRAMING ===\n";
    std::cout<<"add_sub_all true:"<<add_sub_all<true, 5, 4, 3, 2>::res_value<<std::endl;
    std::cout<<"add_sub_all false:"<<add_sub_all<false, 25, 4, 1>::res_value<<std::endl;
    std::cout<<"add_all:"<<add_all_<5, 4, 3, 2>::value<<std::endl;
    std::cout<<"sub_all:"<<sub_all_<25, 4, 2, 4>::value<<std::endl;

    std::cout<<sv<<std::endl;
    std::cout<<sv2<<std::endl;

    // 测试 std::integer_sequence
    std::cout << "Testing std::integer_sequence:" << std::endl;
    print_sequence(std::integer_sequence<int, 1, 3, 5, 7, 9>{});

    // 测试 std::index_sequence 打印元组
    auto myTuple = std::make_tuple(10, 20.5, "hello");
    std::cout << "Printing tuple: ";
    print_tuple(myTuple);
}
}

int main(int argc, char *argv[])
{
    TYPE::test();
    
    APPLICATION::test();

    RULE::test();

    MEATPROGRAMING::test();
    return 0;
}
