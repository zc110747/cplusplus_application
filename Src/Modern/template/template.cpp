/******************************************************************
 * 1.基于范围的for循环
********************************************************************/
#include "template.hpp"
#include <iostream>
#include <vector>

#define FUNCTION_START()  {cout<<__func__<<":\n  ";}{
#define FUNCTION_END()    }{cout<<"\n\n";  }  

using namespace std;

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

    {
      vector<int> v1{1, 2, 3};
      for(const auto &val:v1)
      {
        cout<<val<<" ";
      }
      cout<<" | ";
    }

    FUNCTION_END() 
    return 0;
}