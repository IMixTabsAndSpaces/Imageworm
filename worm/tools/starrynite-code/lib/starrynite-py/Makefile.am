## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I"$(top_srcdir)/lib/starrynite"
AM_CPPFLAGS += -I"$(top_builddir)/lib/gnulib"

pyexec_LTLIBRARIES = starrynite.la

BUILT_SOURCES = starrynitemodule.c

EXTRA_DIST = starrynitemodule.c
MAINTAINERCLEANFILES = starrynitemodule.c starrynitemodule.h

starrynite_la_SOURCES = starrynitemodule.c starrynite.pyx
starrynite_la_CPPFLAGS = $(PYTHON_CPPFLAGS) $(AM_CPPFLAGS)
starrynite_la_LDFLAGS = -module $(PYTHON_LDFLAGS) $(AM_LDFLAGS)
starrynite_la_LIBADD = $(top_builddir)/lib/starrynite/libstarrynite.la

starrynitemodule.c: starrynite.pyx
	$(CYTHON) -o $@ $<
