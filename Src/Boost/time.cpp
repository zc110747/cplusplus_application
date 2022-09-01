
#include "include.hpp"
#include <boost/timer.hpp>

using namespace boost;

int boost_time_process(void)
{
    timer t;

    std::cout<<"time min:"<<t.elapsed_min()<<std::endl;
    std::cout<<"time max:"<<t.elapsed_max()<<std::endl;
    std::cout<<"elapsed:"<<t.elapsed()<<std::endl;
    return 0;
}

