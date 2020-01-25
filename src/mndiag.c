#include "mndiag_private.h"

#define REGCODES_INITIALIZER(n, c) {n, #c, c}
static mndiag_code_info_t registered_libraries[] = {
    REGCODES_INITIALIZER("mncommon", MNDIAG_LIBRARY_MNCOMMON),
    REGCODES_INITIALIZER("mnl4c", MNDIAG_LIBRARY_MNL4C),
    REGCODES_INITIALIZER("mnyaml", MNDIAG_LIBRARY_MNYAML),
    REGCODES_INITIALIZER("mnmath", MNDIAG_LIBRARY_MNMATH),
    REGCODES_INITIALIZER("mnthr", MNDIAG_LIBRARY_MNTHR),
    REGCODES_INITIALIZER("mnapp", MNDIAG_LIBRARY_MNAPP),
    REGCODES_INITIALIZER("mnamqp", MNDIAG_LIBRARY_MNAMQP),
    REGCODES_INITIALIZER("mnpq", MNDIAG_LIBRARY_MNPQ),
    REGCODES_INITIALIZER("mnredis", MNDIAG_LIBRARY_MNREDIS),
    REGCODES_INITIALIZER("mnfcgi", MNDIAG_LIBRARY_MNFCGI),
    REGCODES_INITIALIZER("mnhtools", MNDIAG_LIBRARY_MNHTOOLS),
    REGCODES_INITIALIZER("mnsvnup", MNDIAG_LIBRARY_MNSVNUP),
    REGCODES_INITIALIZER("mnspinfo", MNDIAG_LIBRARY_MNSPINFO),
    REGCODES_INITIALIZER("mnlkit", MNDIAG_LIBRARY_MNLKIT),
    REGCODES_INITIALIZER("mnlipc", MNDIAG_LIBRARY_MNLIPC),
    REGCODES_INITIALIZER("qwe", MNDIAG_LIBRARY_QWE),
    REGCODES_INITIALIZER("<global>", MNDIAG_LIBRARY_GLOBAL),
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
