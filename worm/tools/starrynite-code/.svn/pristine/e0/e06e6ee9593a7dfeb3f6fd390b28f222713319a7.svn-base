AM_CPPFLAGS = -I"$(top_builddir)/lib/gnulib" -I"$(top_srcdir)/lib/starrynite"

bin_PROGRAMS = starrynite
bin_SCRIPTS =

if BUILD_PYTHON
bin_SCRIPTS += starrynite-py
endif

if BUILD_XML
bin_PROGRAMS += starrynite-dump
endif

EXTRA_DIST = starrynite-py.in
CLEANFILES = starrynite-py

starrynite_SOURCES = starrynite.c
starrynite_LDADD = $(top_builddir)/lib/starrynite/libstarrynite.la

starrynite_dump_SOURCES = starrynite-dump.c
starrynite_dump_LDADD = \
  $(top_builddir)/lib/starrynite/libstarrynite.la \
  $(top_builddir)/lib/starrynite-xml/libstarrynite-xml.la \
 # end list
starrynite_dump_CPPFLAGS = $(AM_CPPFLAGS) -I"$(top_srcdir)/lib/starrynite-xml"
EXTRA_starrynite_dump_SOURCES = dummy.cxx
 
do_subst = sed -e 's,[@]pyexecdir[@],$(pyexecdir),g' \
    -e 's,[@]PYTHON[@],$(PYTHON),g' \
    -e 's,[@]PACKAGE_VERSION[@],$(PACKAGE_VERSION),g'

starrynite-py: starrynite-py.in Makefile
	$(do_subst) < $< > $@
	chmod +x $@
	
dummy.cxx:
	touch $@
