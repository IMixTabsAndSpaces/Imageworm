## Makefile.am -- Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I"$(top_builddir)/lib/gnulib" -I"$(top_srcdir)/lib/starrynite"

noinst_LTLIBRARIES = libstarrynite-xml.la

EXTRA_DIST = \
  frame.xsd \
  types.xsd \
  movie.xsd \
  status.xsd \
 # end list

CLEANFILES = \
  frame.cxx \
  frame.hxx \
  movie.cxx \
  movie.hxx \
  types.cxx \
  types.hxx \
  status.hxx \
  status.cxx \
 # end list

BUILT_SOURCES = frame.cxx frame.hxx types.cxx types.hxx movie.cxx \
  movie.hxx status.cxx status.hxx

libstarrynite_xml_la_SOURCES = \
  frame.cxx \
  types.cxx \
  frame.hxx \
  types.hxx \
  movie.cxx \
  movie.hxx \
  status.hxx \
  status.cxx \
  run_starrynite_dump_helper.c \
  run_starrynite_dump_helper.h \
  run_starrynite_dump.c \
  run_starrynite_dump.h \
  run_starrynite_helpers.c \
  run_starrynite_helpers.h \
  xml_dump_status.cxx \
  xml_dump_status.h \
 # end list

libstarrynite_xml_la_LIBADD = $(top_builddir)/lib/gnulib/libgnu.la \
  $(top_builddir)/lib/starrynite/libstarrynite.la \
  -lxerces-c

frame.cxx: frame.xsd
	$(XSDCXX) cxx-tree \
	    --generate-serialization \
	    --namespace-map 'http://starrynite.sf.net/ns/1=starrynite' \
	    $<

frame.hxx: frame.cxx
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
	
movie.cxx: movie.xsd
	$(XSDCXX) cxx-tree \
	    --generate-serialization \
	    --namespace-map 'http://starrynite.sf.net/ns/1=starrynite' \
	    $<

movie.hxx: movie.cxx
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi	
	
status.cxx: status.xsd
	$(XSDCXX) cxx-tree \
	    --generate-serialization \
	    --namespace-map 'http://starrynite.sf.net/ns/1=starrynite' \
	    $<

status.hxx: status.cxx
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi
	
types.cxx: types.xsd
	$(XSDCXX) cxx-tree \
	    --generate-serialization \
	    --namespace-map 'http://starrynite.sf.net/ns/1=starrynite' \
	    $<

types.hxx: types.cxx
	@if test -f $@; then :; else \
	  rm -f $<; \
	  $(MAKE) $(AM_MAKEFLAGS) $<; \
	fi