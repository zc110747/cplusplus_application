/******************************************************************

********************************************************************/
#include "001.megre.hpp"
#include "002.reserved.hpp"
#include "003.linelist.hpp"
#include "004.singlelist.hpp"
#include "005.doublelist.hpp"
#include "006.mostgive.hpp"
#include "007.mediatwo.hpp"

int main(int argc, char *argv[])
{
    ALG_MEGRE::test();

    ALG_RESERVED::test();

    ALG_LINE_LIST::test();

    ALG_SINGAL_LIST::test();

    ALG_DOUBLE_LIST::test();

    ALG_MOSTGIVE::test();

    ALG_MEDIATWO::test();
    return 0;
}