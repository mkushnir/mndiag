#include <assert.h>
#include <err.h>
#include <ctype.h>
#include <getopt.h>
#include <libgen.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "config.h"

#ifdef HAVE_MALLOC_H
#   include <malloc.h>
#endif

#include <mndiag.h>

#define FAIL(s) do {perror(s); abort(); } while (0)

#ifndef NDEBUG
const char *_malloc_options = "AJ";
#endif

typedef struct _mndiag_file_info {
    const char *path;
    struct stat sb;
    int fd;
} mndiag_file_info_t;

static int verbose;
static char *cout;
static char *hout;
static char *lib;
static char *clist;
static mndiag_file_info_t *file_infos;

static struct option optinfo[] = {
#define MNDIAGEN_OPT_HELP       0
    {"help", no_argument, NULL, 'h'},
#define MNDIAGEN_OPT_VERSION    1
    {"version", no_argument, NULL, 'V'},
#define MNDIAGEN_OPT_COUT       2
    {"cout", required_argument, NULL, 'C'},
#define MNDIAGEN_OPT_HOUT       3
    {"hout", required_argument, NULL, 'H'},
#define MNDIAGEN_OPT_LIB        4
    {"lib", required_argument, NULL, 'L'},
#define MNDIAGEN_OPT_CLIST      5
    {"clist", required_argument, NULL, 'S'},
#define MNDIAGEN_OPT_VERBOSE    6
    {"verbose", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0},
};


static void
usage(char *p)
{
    printf("Usage: %s OPTIONS\n"
"\n"
"Options:\n"
"  --help|-h                    Show this message and exit.\n"
"  --lib=NAME|-LNAME            Library name. Required.\n"
"  --clist=PATH|-sPATH          Class list. Required.\n"
"  --hout=PATH|-HPATH           Output header. Default <libname>-diag.h.\n"
"  --cout=PATH|-CPATH           Output source. Default <libname>-diag.c.\n"
"  --verbose|-v                 Increase verbosity.\n"
,
        basename(p));
}

/*
 * MNDIAG_QWE
 * MNDIAG_ASD
 * MNDIAG_ZXC
 * MNDIAG_XCVQWE
 */

static int
symcmp(const char **a, const char **b)
{
    if (*a == NULL) {
        if (*b == NULL) {
            return 0;
        } else {
            return -1;
        }
    } else {
        if (*b == NULL) {
            return 1;
        } else {
            return strcmp(*a, *b);
        }
    }
}


static void
sym_validate(char *sym, struct stat *clistsb, FILE *coutfp, FILE *houtfp)
{
    static char *buf;
    size_t symlen;
    mndiag_file_info_t *f;
    struct stat coutsb, houtsb;

    if (fstat(fileno(coutfp), &coutsb) != 0) {
        return;
    }

    if (fstat(fileno(houtfp), &houtsb) != 0) {
        return;
    }

    symlen = strlen(sym);

    for (f = file_infos; f != NULL && f->path != NULL; ++f) {
        if (f->fd == -1) {
            //printf("no file %s\n", f->path);
        } else {
            ssize_t nread;
            char *p;

            if (f->sb.st_ino == coutsb.st_ino) {
                continue;
                printf("f->sb.st_ino=%d for file cout\n", f->sb.st_ino);
            }
            if (f->sb.st_ino == houtsb.st_ino) {
                continue;
                printf("f->sb.st_ino=%d for file hout\n", f->sb.st_ino);
            }
            if (f->sb.st_ino == clistsb->st_ino) {
                continue;
                printf("f->sb.st_ino=%d for file clist\n", f->sb.st_ino);
            }

            if ((buf = realloc(buf, f->sb.st_size + 1)) == NULL) {
                errx(1, "realloc");
            }
            (void)lseek(f->fd, 0, SEEK_SET);
            if ((nread = read(f->fd, buf, f->sb.st_size)) < 0) {
                perror("read");
                continue;
            }
            assert(nread <= f->sb.st_size);
            buf[nread] = '\0';
            //printf("file %s\n", f->path);
            //printf("buf=%s", buf);
            if ((p = strstr(buf, sym)) != NULL) {
                char *endp;

                /*
                 * word boundary check (dumb)
                 */
                endp = p + symlen;
                if ((p == buf || !isalnum(*(p - 1))) &&
                    (endp >= (buf + nread) || !isalnum(*(endp + 1)))) {
                    //printf("found: ");
                    goto end;
                } else {
                    //printf("part of\n");
                    goto end;
                }
            } else {
            }
        }
    }
    // not found
    if (verbose >= 1) {
        fprintf(stderr, "Skipping %s (not found in files)\n", sym);
    }
    *sym = '\0';

end:
    ;
    //printf("sorted %s\n", sym);
}


//void
//mndiag_local_str(int code, char *buf, size_t sz)
//{
//    const char *lib;
//    const char *class;
//    int message;
//    lib = mndiag_library_name(MNDIAG_GET_LIBRARY(code));
//    class = mndiag_local_class_name(MNDIAG_GET_CLASS(code));
//    message = MNDIAG_GET_MESSAGE(code);
//    (void)snprintf(buf, sz, "%s:%s+%d[%08x]", lib, class, message, code);
//}


static void
clist_write(const char *lib,
            FILE *coutfp,
            FILE *houtfp,
            char **syms,
            int symlen)
{
    int i, j;
    char *p;
    char *libupper;


    if ((libupper = strdup(lib)) == NULL) {
        errx(1, "strdup");
    }
    for (p = libupper; *p != '\0'; ++p) {
        *p = toupper(*p);
    }

    //printf("lib=%s cout=%s hout=%s\n", lib, cout, hout);

    fprintf(houtfp, "#ifndef %s_DIAG_H\n", libupper);
    fprintf(houtfp, "#define %s_DIAG_H\n", libupper);
    fprintf(houtfp, "/* Autogenerated by %s */\n", PACKAGE_STRING);
    fprintf(houtfp, "#include <mndiag.h>\n");
    fprintf(houtfp, "#ifdef __cplusplus\n");
    fprintf(houtfp, "extern \"C\" {\n");
    fprintf(houtfp, "#endif\n");

    fprintf(coutfp, "#include <stdio.h>\n");
    fprintf(coutfp, "#include <mndiag.h>\n");

    //fprintf(coutfp, "#include \"%s\"\n", hout);

    fprintf(houtfp, "#define FAIL(s) do { perror(s); abort(); } while (0)\n");
    fprintf(coutfp, "static mndiag_code_info_t classes[] = {\n");

    for (i = 0, j = 1; i < symlen; ++i) {
        if (strlen(syms[i]) > 0) {

            fprintf(houtfp,
                    "#define MNDIAG_CLASS_%s_%s (%d)\n",
                    libupper,
                    syms[i],
                    j);
            fprintf(houtfp,
                    "#define %s MNDIAG_INTERNAL_CODE(MNDIAG_LIBRARY_%s, "
                    "MNDIAG_CLASS_%s_%s, 0)\n",
                    syms[i],
                    libupper,
                    libupper,
                    syms[i]);

            fprintf(coutfp,
                    "#define MNDIAG_CLASS_%s_%s (%d)\n",
                    libupper,
                    syms[i],
                    j);
            fprintf(coutfp,
                    "#define %s MNDIAG_INTERNAL_CODE(MNDIAG_LIBRARY_%s, "
                    "MNDIAG_CLASS_%s_%s, 0)\n",
                    syms[i],
                    libupper,
                    libupper,
                    syms[i]);


            fprintf(coutfp,
                    "   {\"%s\", \"%s\", MNDIAG_CLASS_%s_%s},\n",
                    syms[i],
                    syms[i],
                    libupper,
                    syms[i]);

            ++j;
        }
    }

    fprintf(houtfp, "#define mndiag_local_str mndiag_%s_str\n", lib);
    fprintf(houtfp,
            "#define mndiag_local_class_name mndiag_%s_class_name\n",
            lib);
    fprintf(houtfp, "const char *mndiag_%s_class_name(int);\n", lib);
    fprintf(houtfp, "void mndiag_%s_str(int, char *, size_t);\n", lib);

    fprintf(houtfp, "#ifdef __cplusplus\n");
    fprintf(houtfp, "}\n");
    fprintf(houtfp, "#endif\n");
    fprintf(houtfp, "#endif\n");



    fprintf(coutfp, "};\n");

    fprintf(coutfp, "const char *\n");
    fprintf(coutfp, "mndiag_%s_class_name(int ccode)\n", lib);
    fprintf(coutfp, "{\n");
    fprintf(coutfp, "    int i;\n");
    fprintf(coutfp, "    for (i = 0; i < (int)(sizeof(classes) / sizeof(classes[0])); ++i) {\n");
    fprintf(coutfp, "        if (ccode == classes[i].code) {\n");
    fprintf(coutfp, "            return classes[i].name;\n");
    fprintf(coutfp, "        }\n");
    fprintf(coutfp, "    }\n");
    fprintf(coutfp, "    return \"<unknown>\";\n");
    fprintf(coutfp, "}\n");


    fprintf(coutfp, "void\n");
    fprintf(coutfp, "mndiag_%s_str(int code, char *buf, size_t sz)\n", lib);
    fprintf(coutfp, "{\n");
    fprintf(coutfp, "    const char *lib;\n");
    fprintf(coutfp, "    const char *cname;\n");
    fprintf(coutfp, "    int message;\n");
    fprintf(coutfp, "    lib = mndiag_library_name(MNDIAG_GET_LIBRARY(code));\n");
    fprintf(coutfp, "    cname = mndiag_%s_class_name(MNDIAG_GET_CLASS(code));\n", lib);
    fprintf(coutfp, "    message = MNDIAG_GET_MESSAGE(code);\n");
    fprintf(coutfp, "    (void)snprintf(buf, sz, \"%%s:%%s+%%d[%%08x]\", lib, cname, message, code);\n");
    fprintf(coutfp, "}\n");


}


static void
clist_do(const char *path,
         const char *lib,
         const char *cout,
         const char *hout)
{
    struct stat sb;
    char *buf;
    int fd;
    char *s0, *s1;
    int i, n;
    char **syms;
    FILE *coutfp, *houtfp;

    if (lstat(path, &sb) != 0) {
        errx(1, "lstat error");
    }
    if ((fd = open(path, O_RDONLY)) < 0) {
        errx(1, "open");
    }
    if ((buf = malloc(sb.st_size) + 1) == NULL) {
        errx(1, "malloc");
    }
    if (read(fd, buf, sb.st_size) < 0) {
        errx(1, "read");
    }
    buf[sb.st_size] = '\0';
    close(fd);

    for (n = 0, s0 = buf, s1 = strchr(s0, '\n');
         s1 != NULL;
         s0 = ++s1, s1 = strchr(s0, '\n')) {
        ++n;
    }
    //printf("n=%d\n", n);

    if ((syms = malloc(sizeof(char *) * n)) == NULL) {
        errx(1, "malloc");
    }
    for (i = 0, s0 = buf, s1 = strchr(s0, '\n');
         s1 != NULL;
         s0 = ++s1, s1 = strchr(s0, '\n'), ++i) {
        syms[i] = s0;
        *s1 = '\0';
        //printf("sym %s\n", s0);
    }
    syms[i] = s0;
    //printf("i=%d\n", i);

    qsort(syms,
          n,
          sizeof(char *),
          (int(*)(const void *, const void *))symcmp);


    if ((coutfp = fopen(cout, "w")) == NULL) {
        errx(1, "fopen");
    }
    if ((houtfp = fopen(hout, "w")) == NULL) {
        errx(1, "fopen");
    }

    for (i = 0; i < n; ++i) {
        if (strlen(syms[i]) > 0) {
            sym_validate(syms[i], &sb, coutfp, houtfp);
        }
    }

    clist_write(lib, coutfp, houtfp, syms, n);

    fclose(coutfp);
    fclose(houtfp);
}


int
main(int argc, char **argv)
{
    int i;
    int ch;
    int idx;
    //int libid;
    //char *libidsym;


#ifdef HAVE_MALLOC_H
#   ifndef NDEBUG
    /*
     * malloc options
     */
    if (mallopt(M_CHECK_ACTION, 1) != 1) {
        FAIL("mallopt");
    }
    if (mallopt(M_PERTURB, 0x5a) != 1) {
        FAIL("mallopt");
    }
#   endif
#endif

    while ((ch = getopt_long(argc, argv, "C:H:hL:S:v", optinfo, &idx)) != -1) {
        switch (ch) {
        case 'h':
            usage(argv[0]);
            exit(0);
            break;

        case 'C':
            cout = strdup(optarg);
            break;

        case 'H':
            hout = strdup(optarg);
            break;

        case 'L':
            lib = strdup(optarg);
            break;

        case 'S':
            clist = strdup(optarg);
            break;

        case 'v':
            ++verbose;
            break;

        default:
            usage(argv[0]);
            exit(1);
        }
    }

    if (lib == NULL) {
        errx(1, "--lib cannot be empty. See %s --help", basename(argv[0]));
    }

    if (clist == NULL) {
        errx(1, "--clist cannot be empty. See %s --help", basename(argv[0]));
    }

    if (cout == NULL) {
        char *buf;
        size_t sz;

        sz = strlen(lib) + 32;
        if ((buf = malloc(sz)) == NULL) {
            errx(1, "malloc");
        }
        (void)snprintf(buf, sz, "%s-diag.c", lib);
        cout = buf;
    }

    if (hout == NULL) {
        char *buf;
        size_t sz;

        sz = strlen(lib) + 32;
        if ((buf = malloc(sz)) == NULL) {
            errx(1, "malloc");
        }
        (void)snprintf(buf, sz, "%s-diag.c", lib);
        hout = buf;
    }

    argc -= optind;
    argv += optind;

    if ((file_infos = malloc(
                    sizeof(mndiag_file_info_t) * (argc + 1))) == NULL) {
        errx(1, "malloc");
    }

    for (i = 0; i < argc; ++i) {
        mndiag_file_info_t *f = file_infos + i;
        f->path = argv[i];
        f->fd = -1;
        if (lstat(f->path, &f->sb) != 0) {
            continue;
        }
        if ((f->fd = open(f->path, O_RDONLY)) != -1) {
            continue;
        }
    }
    mndiag_file_info_t *f = file_infos + i;
    f->path = NULL;

    clist_do(clist, lib, cout, hout);

    //libid = mndiag_library_id(lib);
    //libidsym = mndiag_library_id_symbol(lib);
}
