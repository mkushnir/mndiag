#include "mndiag_private.h"

#define REGCODES_INITIALIZER(n, c) {n, #c, c}
static mndiag_code_info_t registered_libraries[] = {
    REGCODES_INITIALIZER("mrkcommon", MNDIAG_LIBRARY_MRKCOMMON),
    REGCODES_INITIALIZER("mrkl4c", MNDIAG_LIBRARY_MRKL4C),
    REGCODES_INITIALIZER("mrkyaml", MNDIAG_LIBRARY_MRKYAML),
    REGCODES_INITIALIZER("mnmath", MNDIAG_LIBRARY_MNMATH),
    REGCODES_INITIALIZER("mrkthr", MNDIAG_LIBRARY_MRKTHR),
    REGCODES_INITIALIZER("mrkapp", MNDIAG_LIBRARY_MRKAPP),
    REGCODES_INITIALIZER("mrkamqp", MNDIAG_LIBRARY_MRKAMQP),
    REGCODES_INITIALIZER("mrkpq", MNDIAG_LIBRARY_MRKPQ),
    REGCODES_INITIALIZER("mnredis", MNDIAG_LIBRARY_MNREDIS),
    REGCODES_INITIALIZER("mnfcgi", MNDIAG_LIBRARY_MNFCGI),
    REGCODES_INITIALIZER("mnhtools", MNDIAG_LIBRARY_MNHTOOLS),
    REGCODES_INITIALIZER("mrksvnup", MNDIAG_LIBRARY_MRKSVNUP),
    REGCODES_INITIALIZER("spinfo", MNDIAG_LIBRARY_SPINFO),
    REGCODES_INITIALIZER("mrklkit", MNDIAG_LIBRARY_MRKLKIT),
    REGCODES_INITIALIZER("qwe", MNDIAG_LIBRARY_QWE),
    REGCODES_INITIALIZER("<other>", MNDIAG_LIBRARY_OTHER),
};

unsigned
mndiag_spec_version(void)
{
    return MNDIAG_SPEC_VERSION;
}

const char *
mndiag_library_name(int lib)
{
    unsigned i;

    for (i = 0; i < sizeof(registered_libraries) / sizeof(registered_libraries[0]); ++i) {
        if (lib == registered_libraries[i].code) {
            return registered_libraries[i].name;
        }
    }
    return "<unknown>";
}


const char *
mndiag_library_code_name(int lib)
{
    unsigned i;

    for (i = 0; i < sizeof(registered_libraries) / sizeof(registered_libraries[0]); ++i) {
        if (lib == registered_libraries[i].code) {
            return registered_libraries[i].code_name;
        }
    }
    return "<unknown>";
}
