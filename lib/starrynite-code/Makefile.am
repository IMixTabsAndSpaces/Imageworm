AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = m4/gnulib-cache.m4 $(DX_CONFIG)

SUBDIRS = lib bin tests .

include $(srcdir)/doxygen.am

MOSTLYCLEANFILES = $(DX_CLEANFILES)
