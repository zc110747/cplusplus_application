
#include <iostream>
#include <tuple>
#include <algorithm>
#include <optional>

namespace TEMPLATE_LAMBDA 
{
    template <typename Tuple, size_t... I>
    auto get_tuple(Tuple &tp, std::index_sequence<I...>) 
    {
        return std::make_tuple(std::optional(std::get<I>(tp))...);
    }

    int test_run() 
    {
        std::tuple<int, double> tp{2, 2.5};
        auto tp1 = get_tuple(tp, std::make_index_sequence<2>{});
        std::optional<int> val1 = get<0>(tp1);
        auto val2 = get<1>(tp1);
        std::cout<<val1.value()<<" "<<val2.value()<<std::endl;

        //基于lambda的模板(C++ 20)
        auto tp2 = []<typename Tuple, size_t... I>(Tuple &tp, std::index_sequence<I...>) {
            return std::make_tuple(std::optional(std::get<I>(tp))...);
        }(tp, std::make_index_sequence<2>{});  
        std::cout<<get<0>(tp2).value()<<" "<<get<1>(tp2).value()<<std::endl;

        auto tp3 = [&tp]<size_t... I>(std::index_sequence<I...>) {
            return std::make_tuple(std::optional(std::get<I>(tp))...);
        }(std::make_index_sequence<2>{});
        std::cout<<get<0>(tp3).value()<<" "<<get<1>(tp3).value()<<std::endl; 
        
        return 0;
    }
}