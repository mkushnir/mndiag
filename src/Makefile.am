# consider headers:
#   - include_HEADERS
#   - nobase_include_HEADERS
#   - pkginclude_HEADERS
#   - dist_HEADERS
#   - nodist_HEADERS
#   - noinst_HEADERS

CLEANFILES = *.core core
#CLEANFILES += *.in
AM_MAKEFLAGS = -s
AM_LIBTOOLFLAGS = --silent

lib_LTLIBRARIES = libmndiag.la

if DEVTOOLS
bin_PROGRAMS = mndiagen
endif

nobase_include_HEADERS = mndiag.h
noinst_HEADERS = mndiag_private.h

libmndiag_la_SOURCES = mndiag.c

if DEVTOOLS
mndiagen_SOURCES = mndiagen.c
endif

DEBUG_LD_FLAGS =
if DEBUG
DEBUG_CC_FLAGS = -g -O0 @CC_DEBUG@
DEBUG_LD_FLAGS += @LIBTOOL_NO_INSTALL@
else
DEBUG_CC_FLAGS = -DNDEBUG -O3 -fomit-frame-pointer
if LTO
DEBUG_CC_FLAGS += @CC_LTO@
DEBUG_LD_FLAGS += @LD_LTO@
endif
endif

if ALLSTATIC
libmndiag_la_LDFLAGS = -all-static
if DEVTOOLS
mndiagen_LDFLAGS = -all-static
endif
else
libmndiag_la_LDFLAGS =
if DEVTOOLS
mndiagen_LDFLAGS =
endif
endif

libmndiag_la_CFLAGS = $(DEBUG_CC_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)

libmndiag_la_LDFLAGS += $(DEBUG_LD_FLAGS) -version-info 0:0:0 -L$(libdir)
libmndiag_la_LIBADD =
#libmndiag_la_LDFLAGS = -version-info 0:0:0 -L$(libdir) -lmncommon
#libmndiag_la_LDFLAGS = -all-static
#libmndiag_la_LDFLAGS = -all-static -Wl,-Bdynamic,-L$(libdir),-lfoo -lqwe,-Bstatic

if DEVTOOLS
mndiagen_CFLAGS = $(DEBUG_CC_FLAGS) -Wall -Wextra -Werror -std=c99 @_GNU_SOURCE_MACRO@ @_XOPEN_SOURCE_MACRO@ -I$(top_srcdir)/src -I$(top_srcdir) -I$(includedir)
mndiagen_LDFLAGS += $(DEBUG_LD_FLAGS) -L$(libdir)
mndiagen_LDADD =
endif

run: all
	for i in $(bin_PROGRAMS); do if test -x ./$$i; then LD_LIBRARY_PATH=$(libdir) ./$$i; fi; done;

testrun:
	for i in $(SUBDIRS); do if test "$$i" != "."; then cd $$i && $(MAKE) testrun && cd ..; fi; done;
