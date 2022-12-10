
/******************************************************************
第十九章 静态断言(static_assert)
********************************************************************/
#include "assert.hpp"

class X
{
public:
    int a;
};

class VX
{
    virtual void func(){
    }
};

int assert_process(void)
{
    static_assert(true, "Not Right!");
    static_assert(sizeof(X) == sizeof(X::a), "Not Equal!");
    static_assert(sizeof(VX) == sizeof(void *), "not equal ptr");
    return 0;
}