
#include "template.hpp"
#include <iostream>

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
    cout<<add_t<1, 2>::result<<endl;
    return 0;
}