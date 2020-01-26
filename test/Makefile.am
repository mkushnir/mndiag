CLEANFILES = *.core core
#CLEANFILES += *.in
AM_MAKEFLAGS = -s
AM_LIBTOOLFLAGS = --silent

# consider headers:
#   - include_HEADERS
#   - nobase_include_HEADERS
#   - pkginclude_HEADERS
#   - dist_HEADERS
#   - nodist_HEADERS
#   - noinst_HEADERS
noinst_HEADERS = unittest.h

noinst_PROGRAMS = testfoo gendata

if CMOCKA
noinst_PROGRAMS += testbar
endif

EXTRA_DIST = runscripts

if DEBUG
DEBUG_FLAGS = -g -O0 @CC_DEBUG@
else
DEBUG_FLAGS = -DNDEBUG -O3 -fomit-frame-pointer
endif

testfoo_SOURCES = testfoo.c
testfoo_CFLAGS = $(DEBUG_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/test -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)
#testfoo_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag -L$(libdir)
testfoo_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag

if CMOCKA
testbar_SOURCES = testbar.c
testbar_CFLAGS = $(DEBUG_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/test -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)
#testbar_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag -L$(libdir) -lcmocka
testbar_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag -lcmocka
endif

gendata_SOURCES = gendata.c
gendata_CFLAGS = $(DEBUG_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/test -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)
gendata_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag -L$(libdir)
#gendata_LDFLAGS = -L$(top_srcdir)/src/.libs -lmndiag

testfoo-run: testfoo
	@if test -x $>; \
	    then LD_LIBRARY_PATH=$(libdir) ./$> $${$(>)_ARGS}; \
	    else true; \
	fi

testrun: all
	for i in $(noinst_PROGRAMS); do if test -x ./$$i; then LD_LIBRARY_PATH=$(libdir) ./$$i; fi; done;
