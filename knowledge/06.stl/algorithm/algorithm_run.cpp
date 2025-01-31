/************************************************************************************
 * algorithm
 * adjacent_find: 搜索相等或满足指定条件的两个相邻元素
***********************************************************************************/
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template<class T>
typename std::enable_if<is_integral<T>::value, bool>::type
is_odd(T i)
{
    return bool(i%2);
}

namespace ADJACENT_FIND
{
    class run_function
    {
    public:
        virtual ~run_function() {}
        virtual bool operator()(int temp1, int temp2) {
            return (2*temp1 == temp2);  
        }
    };

    void adjacent_find_run(void)
    {
        vector<int> val = {1, 3, 3, 7, 5, 5, 4, 8};
        auto iter = val.begin();
        decltype(iter) result;
        run_function func;

        //打印第一个满足要求的对象
        result = adjacent_find(iter, val.end());
        if (result != val.end()) {
            cout<<*result<<" | ";
        }

        //打印所有满足要求的对象
        iter = val.begin();
        while((result = adjacent_find(iter, val.end())) != val.end())
        {
            iter = ++result;
            cout<<*result<<" | ";
        }

        //使用lambda打印第一个满足的对象
        iter = val.begin();
        result = adjacent_find(iter, val.end(), [](int temp1, int temp2){
            return (2*temp1 == temp2);
        });
        if (result != val.end()) {
            cout<<*result<<" | ";
        }

        //使用object打印打一个满足的对象
        result = adjacent_find(iter, val.end(), func);
        if (result != val.end()) {
            cout<<*result<<" | ";
        }
        cout<<endl;
    }
}

namespace ALL_OF
{
    class function
    {
    public:
        virtual ~function() {}
        virtual bool operator()(int elem) {
            return is_odd(elem);
        }
    };

    void all_of_run(void)
    {
        vector<int> val = {1, 3, 5, 7, 9};
        bool result;
        function func;

        result = all_of(val.begin(), val.end(), [](int elem){
            return (elem%2 != 0);
        });
        cout<<std::boolalpha<<result<<" | ";

        result = all_of(val.begin(), val.end(), func);
        cout<<std::boolalpha<<result<<" | ";

        cout<<endl;
    }
}

int main(void)
{
    ADJACENT_FIND::adjacent_find_run();

    ALL_OF::all_of_run();
    return 0;
}