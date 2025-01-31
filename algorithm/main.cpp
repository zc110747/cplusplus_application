/******************************************************************

********************************************************************/
#include "megre.hpp"
#include "reserved.hpp"
#include "linelist.hpp"
#include "SingleList.hpp"

namespace DOUBLE_LIST
{
    void test_run();
}

namespace MAXNUM
{
    void run();
}

int main(int argc, char *argv[])
{
    MEGRE::test_run();

    RESERVED::test_run();

    LINE_LIST::test_run();

    SINGLE_LIST::test_run();

    DOUBLE_LIST::test_run();

    MAXNUM::run();
    return 0;
}