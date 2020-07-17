/**********************************************************
 * 说明:tuple
 * 用于存储不同类型的的元组
************************************************************/
#include <tuple>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &vector_val)
{
    for(auto val:vector_val)
    {
        os<<val<<" ";
    }
    return os;
}

template<typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, std::tuple<T1, T2> &tuple_val)
{
    os<<std::get<0>(tuple_val)<<" "<<std::get<1>(tuple_val)<<" ";
    return os;
}

int main(int argc, char* argv[])
{       
    std::tuple<int, float, std::string> tuple_val0 = std::make_tuple(1, 0.5, "test");
    std::cout<<"First:"<<std::get<0>(tuple_val0)<<std::endl;
    std::cout<<"Second:"<<std::get<1>(tuple_val0)<<std::endl;
    std::cout<<"Third:"<<std::get<2>(tuple_val0)<<std::endl;
    std::cout<<std::tuple_size<decltype(tuple_val0)>::value<<std::endl;

    std::tuple<std::string, std::vector<int> > tuple_val1{"hello", {1, 2, 3, 4}};
    std::cout<<std::get<1>(tuple_val1)<<std::endl;

    std::tuple<int, float, std::string> tuple_val2(tuple_val0);
    std::cout<<"First:"<<std::get<0>(tuple_val2)<<std::endl;
    
    std::tuple<int, float> tuple_val3(std::make_pair(1, (float)0.2));
    std::cout<<"tuple3 val:"<<tuple_val3<<std::endl;

    std::tuple<int, float> tuple_val4;
    tuple_val4.swap(tuple_val3);
    std::cout<<"tuple4 val:"<<tuple_val4<<std::endl;
}
