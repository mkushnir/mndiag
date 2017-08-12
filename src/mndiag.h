#ifndef MNDIAG_H_DEFINED
#define MNDIAG_H_DEFINED

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 0x80000000
 *   |_______ library vs application (global)
 *
 * 0x40000000
 *   |_______ library public vs internal
 */

/*
 * versoin 0.1 (0000 .. 0001)
 */
#define MNDIAG_SPEC_VERSION  (0x00000001)
#define MNDIAG_BIT_GLOBAL    (1u << (sizeof(int) * 8 - 1))
#define MNDIAG_BIT_PUBLIC    (1u << (sizeof(int) * 8 - 2))
#define MNDIAG_MAX_LIBRARY   (0xffu)
#define MNDIAG_SHF_LIBRARY   (sizeof(int) * 8 - 10)
#define MNDIAG_MAX_CLASS     (0xffu)
#define MNDIAG_SHF_CLASS     (sizeof(int) * 8 - 18)
#define MNDIAG_MAX_MESSAGE   (0x3fffu)
#define MNDIAG_SHF_MESSAGE   (0)


#define MNDIAG_BIT_LIBRARY  \
    (MNDIAG_MAX_LIBRARY << MNDIAG_SHF_LIBRARY)

#define MNDIAG_BIT_CLASS    \
    (MNDIAG_MAX_CLASS << MNDIAG_SHF_CLASS)

#define MNDIAG_BIT_MESSAGE  \
    MNDIAG_MAX_MESSAGE << MNDIAG_SHF_MESSAGE

#define MNDIAG_LIBRARY(n)   \
    (((n) & MNDIAG_MAX_LIBRARY) << MNDIAG_SHF_LIBRARY)

#define MNDIAG_CLASS(n)     \
    (((n) & MNDIAG_MAX_CLASS) << MNDIAG_SHF_CLASS)

#define MNDIAG_MESSAGE(n)   \
    (((n) & MNDIAG_MAX_MESSAGE) << MNDIAG_SHF_MESSAGE)

#define MNDIAG_INTERNAL_CODE(l, c, m)  \
    (MNDIAG_BIT_GLOBAL |               \
     MNDIAG_LIBRARY(l) |               \
     MNDIAG_CLASS(c) |                 \
     MNDIAG_MESSAGE(m))                \


#define MNDIAG_PUBLIC_CODE(l, c, m)    \
    (MNDIAG_INTERNAL_CODE(l, c, m) | MNDIAG_BIT_PUBLIC)

#define MNDIAG_GET_GLOBAL(c) ((c) & MNDIAG_BIT_GLOBAL)

#define MNDIAG_GET_PUBLIC(c) ((c) & MNDIAG_BIT_PUBLIC)

#define MNDIAG_GET_LIBRARY(c)          \
    (((c) & MNDIAG_BIT_LIBRARY) >> MNDIAG_SHF_LIBRARY)

#define MNDIAG_GET_CLASS(c)           \
    (((c) & MNDIAG_BIT_CLASS) >> MNDIAG_SHF_CLASS)

#define MNDIAG_GET_MESSAGE(c)        \
    (((c) & MNDIAG_BIT_MESSAGE) >> MNDIAG_SHF_MESSAGE)


/*
 * Library codes
 */
#define MNDIAG_LIBRARY_MRKCOMMON (0x08)
#define MNDIAG_LIBRARY_MRKL4C    (0x09)
#define MNDIAG_LIBRARY_MRKYAML   (0x0a)
#define MNDIAG_LIBRARY_MNMATH    (0x0b)
#define MNDIAG_LIBRARY_MRKTHR    (0x10)
#define MNDIAG_LIBRARY_MRKAPP    (0x11)
#define MNDIAG_LIBRARY_MRKAMQP   (0x12)
#define MNDIAG_LIBRARY_MRKPQ     (0x13)
#define MNDIAG_LIBRARY_MNREDIS   (0x14)
#define MNDIAG_LIBRARY_MNFCGI    (0x15)
#define MNDIAG_LIBRARY_QWE       (0xf0)

#define MNDIAG_LIBRARY_OTHER     (0xff)


typedef struct _mndiag_code_info {
    const char *name;
    const char *code_name;
    int code;
} mndiag_code_info_t;


unsigned mndiag_spec_version(void);
const char *mndiag_library_name(int);
const char *mndiag_library_code_name(int);

#ifdef __cplusplus
}
#endif
#endif /* MNDIAG_H_DEFINED */
