#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([starrynite],[1.3.3],[starrynite-devel@lists.sourceforge.net])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([build-aux/config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC
gl_EARLY
AC_PROG_CC_C99

if test ! -x "${GPERF}"; then
AC_PATH_PROG([GPERF], [gperf], [no])
fi

AC_ARG_VAR([GPERF], [GNU gperf command])

AC_LANG([C])

# Checks for libraries.
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([tiff], [TIFFOpen], [], [
  echo "ERROR: Required library tiff not found";
  exit 1
  ])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([OS.h inttypes.h limits.h stddef.h stdint.h stdlib.h \
  string.h sys/param.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TM
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_CHOWN
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_CHECK_FUNCS([dup2 fchdir ftruncate getcwd getpagesize lchown mempcpy \
  putenv setenv sqrt stpcpy strchr strcspn strdup strerror strndup strpbrk \
  strrchr strspn strstr wcwidth])

gl_INIT

AM_MAINTAINER_MODE

#AM_CONDITIONAL([MAINTAINER_MODE], [test x$maint = xtrue])
AM_CONDITIONAL([HAVE_GPERF], [test "x$GPERF" != "xno"])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  lib/Makefile
  tests/Makefile
])

AC_OUTPUT
