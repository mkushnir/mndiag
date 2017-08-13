#include <assert.h>
#include <stdio.h>

#include <mndiag.h>

#include "unittest.h"

#ifndef NDEBUG
const char *_malloc_options = "AJ";
#endif

static void
print_code(int lcode, int ccode)
{
    printf("lcode=%04x ccode=%04x\n", lcode, ccode);
    printf("% 5d=%08x\n", 0, MNDIAG_PUBLIC_CODE(lcode, ccode, 0));
    printf("% 5d=%08x\n", 1, MNDIAG_PUBLIC_CODE(lcode, ccode, 1));
    printf("% 5d=%08x\n", 128, MNDIAG_PUBLIC_CODE(lcode, ccode, 128));
    printf("% 5d=%08x\n", 0x80, MNDIAG_PUBLIC_CODE(lcode, ccode, 0x80));
    printf("% 5d=%08x\n", -128, MNDIAG_PUBLIC_CODE(lcode, ccode, -128));
    printf("% 5d=%08x\n", -127, MNDIAG_PUBLIC_CODE(lcode, ccode, -127));
    printf("% 5d=%08x\n", -2, MNDIAG_PUBLIC_CODE(lcode, ccode, -2));
    printf("% 5d=%08x\n", -1, MNDIAG_PUBLIC_CODE(lcode, ccode, -1));
}


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
    print_code(1, 1);
    print_code(1, 1 << 9);
    print_code(1 << 11, 1);
    print_code(1 << 11, 1 << 9);
}

int
main(void)
{
    test0();
    return 0;
}
