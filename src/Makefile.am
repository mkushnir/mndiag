# consider headers:
#   - include_HEADERS
#   - nobase_include_HEADERS
#   - pkginclude_HEADERS
#   - dist_HEADERS
#   - nodist_HEADERS
#   - noinst_HEADERS

BUILT_SOURCES = diag.c diag.h
EXTRA_DIST = $(diags) gen-diag
CLEANFILES = $(BUILT_SOURCES) *.core core
#CLEANFILES += *.in
AM_MAKEFLAGS = -s

lib_LTLIBRARIES = libmndiag.la

bin_PROGRAMS = mndiagen

nobase_include_HEADERS = mndiag.h

libmndiag_la_SOURCES = mndiag.c
nodist_libmndiag_la_SOURCES = diag.c

mndiagen_SOURCES = mndiagen.c

diags = diag.txt

if DEBUG
DEBUG_FLAGS = -g -O0 @CC_DEBUG@
else
DEBUG_FLAGS = -DNDEBUG -O3 -fomit-frame-pointer
endif

libmndiag_la_CFLAGS = $(DEBUG_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)

libmndiag_la_LDFLAGS = -version-info 0:0:0
#libmndiag_la_LDFLAGS = -version-info 0:0:0 -L$(libdir) -lmrkcommon
#libmndiag_la_LDFLAGS = -all-static
#libmndiag_la_LDFLAGS = -all-static -Wl,-Bdynamic,-L$(libdir),-lfoo -lqwe,-Bstatic

mndiagen_CFLAGS = $(DEBUG_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)

diag.c diag.h: $(diags) gen-diag
	$(AM_V_GEN) cat $(diags) | sort -u | ./gen-diag mndiag

run: all
	for i in $(bin_PROGRAMS); do if test -x ./$$i; then LD_LIBRARY_PATH=$(libdir) ./$$i; fi; done;

testrun:
	for i in $(SUBDIRS); do if test "$$i" != "."; then cd $$i && $(MAKE) testrun && cd ..; fi; done;