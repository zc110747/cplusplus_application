/******************************************************************
 * 第二章 内联和嵌套命名空间
********************************************************************/
#include "namespace.hpp"
#include <iostream>

using namespace std;

namespace S1 {
    void foo() {
        cout<<"S1::foo"<<" | ";
    }

    namespace Child1 {
        void foo() {
            cout<<"S1::Child1::foo"<<" | ";
        }
    }

    inline namespace Child2 {
        void foo_c() {
            cout<<"S2::Child1::foo"<<" | ";
        }
    }
}

namespace S2 {
    namespace V1{
        void foo() {
            cout<<"S2::V1::foo"<<" | ";
        }
    }

    inline namespace V2{
        void foo() {
            cout<<"S2::V2::foo"<<" | ";
        }
    }
}

namespace A::B::C{
    void foo() {
        cout<<"A::B::C::foo"<<endl;
    }
}

int namespace_process(void)
{
    S1::foo();
    S2::V1::foo();
    S2::foo();
    S1::Child1::foo();
    S1::foo_c();
    A::B::C::foo();
    return 0;
}
