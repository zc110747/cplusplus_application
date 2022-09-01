
#include "include.hpp"
#include "boost/noncopyable.hpp"
#include "boost/core/ignore_unused.hpp"

using namespace boost;

class empty_class: boost::noncopyable
{
private:
    int a{0};

public:
    empty_class(int v){
        a = v;
    }
    ~empty_class(){}
    void printf(){
        std::cout<<__func__<<":"<<a<<std::endl;
    }
};

int boost_tool_process(void)
{
    empty_class v(1);
    v.printf();

    //boost::noncopyable
    // empty_class v1 = v;
    // v1.printf();
    return 0;
}