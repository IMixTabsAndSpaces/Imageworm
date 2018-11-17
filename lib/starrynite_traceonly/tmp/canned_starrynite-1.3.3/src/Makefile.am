AM_CPPFLAGS = -I$(top_srcdir)/lib -I$(top_builddir)/lib

LDADD = ../lib/libgnu.a

bin_PROGRAMS = starrynite
starrynite_SOURCES = cell-identity.c image-manipulation.c \
  noise-filter.c nuclei.c parameters-tools.c report-lineage.c \
  sliding-box.c starrynite.c tiff-io.c tracing.c utility.c \
  cell-identity.h colors.h image-manipulation.h parameters-hash.h \
  noise-filter.h nuclei.h parameters-tools.h report-lineage.h sliding-box.h \
  tiff-io.h tracing.h types.h utility.h parameters-hash.c \
  parameters-hash.gperf error-codes.h
starrynite_LDFLAGS = -lm

CLEANFILES =

if HAVE_GPERF
BUILT_SOURCES = parameters-hash.c
parameters-hash.c: parameters-hash.gperf parameters-hash.h
	@GPERF@ --output-file="$@" "$<"
endif

if MAINTAINER_MODE
  CLEANFILES += parameters-hash.c
endif
