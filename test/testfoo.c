#include <assert.h>
#include <stdio.h>

#include <mndiag.h>

#include "unittest.h"

#ifndef NDEBUG
const char *_malloc_options = "AJ";
#endif

static void
test0(void)
{
    struct {
        long rnd;
        int in;
        int expected;
    } data[] = {
        {0, 0, 0},
    };
    UNITTEST_PROLOG_RAND;

    FOREACHDATA {
        //TRACE("in=%d expected=%d", CDATA.in, CDATA.expected);
        assert(CDATA.in == CDATA.expected);
    }
    /*
     * public code range in the "global" library space
     */
    printf("% 5d=%08x\n", 0, MNDIAG_PUBLIC_CODE(-1, -1, 0));
    printf("% 5d=%08x\n", 1, MNDIAG_PUBLIC_CODE(-1, -1, 1));
    printf("% 5d=%08x\n", 8191, MNDIAG_PUBLIC_CODE(-1, -1, 8191));
    printf("% 5d=%08x\n", 0x2000, MNDIAG_PUBLIC_CODE(-1, -1, 0x2000));
    printf("% 5d=%08x\n", -8192, MNDIAG_PUBLIC_CODE(-1, -1, -8192));
    printf("% 5d=%08x\n", -8191, MNDIAG_PUBLIC_CODE(-1, -1, -8191));
    printf("% 5d=%08x\n", -2, MNDIAG_PUBLIC_CODE(-1, -1, -2));
    printf("% 5d=%08x\n", -1, MNDIAG_PUBLIC_CODE(-1, -1, -1));
}

int
main(void)
{
    test0();
    return 0;
}
