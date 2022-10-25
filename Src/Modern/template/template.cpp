/******************************************************************
 * 1.模板
 * 函数模板，类模板
 * 2.using关键字
********************************************************************/
#include "template.hpp"
#include <iostream>
#include <map>
#include <string>
#include <algorithm>

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

using namespace std;

static int using_process(void);

template<int a, int b>
struct add_t {
  enum  {
    result = a + b,
  };
};

int template_process(void)
{
    FUNCTION_START()

    cout<<add_t<1, 2>::result<<" | ";

    using_process();

    FUNCTION_END() 
    return 0;
}

template<class T>
struct intMap
{
  typedef map<int, T> type;
};

template<class T>
struct intMapVal
{
  typename intMap<int>::type val;
};

template<class T>
using intMapU = map<int, T>;

static int using_process(void)
{
  {
    using map_iter = map<int, string>::iterator;

    map<int, string> val_map{{1, "one"}, {2, "two"}};

    for(map_iter iter=val_map.begin(); iter!=val_map.end(); iter++)
    {
      cout<<iter->first<<" "<<iter->second<<" | ";
    }
  }

  {
    intMap<int>::type value0{{1, 1}, {2, 2}};
    for(auto& [x, y]:value0)
    {
      cout<<x<<" "<<y<<" | ";
    }

    intMapVal<int> value1{{{1, 2}, {3, 4}}};
    for_each(value1.val.begin(), value1.val.end(), [](auto& val){
      auto& [x, y] = val;
      cout<<x<<" "<<y<<" | ";
    });
  }

  {
    intMapU<int> value{{1, 2}, {3, 4}};
    for(auto& [x, y]:value)
    {
      cout<<x<<" "<<y<<" | ";
    }

  }
  return 0;
}