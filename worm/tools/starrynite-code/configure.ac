#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([starrynite],m4_esyscmd([svnversion -n]),[starrynite-devel@lists.sourceforge.net])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADER([build-aux/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 foreign -Wall -Werror])

#############################################
# Options passed to configure

# Note that these can be retrieved with, for example:
# ac_cv_env_CC_set=
# ac_cv_env_CC_value=
# ac_cv_env_PERL_set=set
# ac_cv_env_PERL_value=/usr/local/bin/perl

AC_ARG_VAR([GPERF], [gperf executable to use])
AC_ARG_VAR([PYTHON], [python executable to use])
AC_ARG_VAR([DOT], [dot executable to use])

# This option disabled.
# AM_MAINTAINER_MODE

AC_ARG_ENABLE([python],
  [AS_HELP_STRING([--enable-python],
    [build Python interface files [default=no]])],
  [enable_python=yes],
  [enable_python=no])

AC_ARG_ENABLE([doxygen],
  [AS_HELP_STRING([--enable-doxygen],
    [build Doxygen-generated documentation [default=no]])],
  [enable_doxygen=yes],
  [enable_doxygen=no])
  
AC_ARG_ENABLE([xml],
  [AS_HELP_STRING([--enable-xml],
    [build XML serialization library using C++ [default=no]])],
  [enable_xml=yes],
  [enable_xml=no])

#############################################
# Checks for programs.

AC_PROG_CC
gl_EARLY

## Ensure that the C compiler supports C99.

AC_PROG_CC_C99
AM_PROG_CC_C_O

if test "${ac_cv_prog_cc_c99}" = "no"; then
cat >&2 << 'EOF'
ERROR: starrynite is written in ISO standard C99, but your compiler does
not appear to support C99. Try again using a modern compiler.

You may specify an alternate compiler by passing 'CC=/path/to/compiler'
as an argument to configure.
EOF
exit 1
fi

AC_LANG([C])
LT_PREREQ([2.2.4])
LT_INIT
AC_PROG_SED
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_CXX

AM_MISSING_PROG([XSDCXX], [xsdcxx])
AM_MISSING_PROG([CYTHON], [cython])

AC_PATH_PROG([DOT], [dot], [:])

if test "${DOT}" != ":"
then
  HAVE_DOT=YES
else
  HAVE_DOT=NO
fi

AC_SUBST([HAVE_DOT])


#############################################
# Check if GPERF is defined in the environment and points to an
# executable file. If not, try to find gperf on the path and set GPERF
# to its absolute location.

if test ! -z "${GPERF}" -a ! -x "${GPERF}"
then # GPERF defined, but not executable
cat >&2 << 'EOF'
ERROR: GPERF was specified as an argument to configure, but is not an
executable file.  Did you mean to do this?

Try running configure again, passing 'GPERF=' as an argument.
EOF
exit 1
fi # GPERF defined, but not executable

AM_MISSING_PROG([GPERF], [gperf])


#############################################
# Check if PYTHON is defined in the environment and points to an
# executable file. If not, try to find python on the path and set PYTHON
# to its absolute location.

if test ! -z "${PYTHON}" -a ! -x "${PYTHON}"
then # PYTHON defined, but not executable
cat >&2 << 'EOF'
ERROR: PYTHON was specified as an argument to configure, but is not an
executable file.  Did you mean to do this?

Try running configure again, passing 'PYTHON=' as an argument.
EOF
exit 1
fi # PYTHON defined, but not executable

AM_MISSING_PROG([PYTHON], [python])


#############################################
## If --enable-python is selected, build the Python interface

if test "x${enable_python}" = "xyes"
then # build Python interface

AC_PYTHON_DEVEL
AM_PATH_PYTHON

fi # build Python interface

#############################################
## If --enable-xml is selected, build the XML generator

if test "x${enable_xml}" = "xyes"
then # build XML generator

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([xsd/cxx/version.hxx xercesc/dom/DOMDocument.hpp],
  [], 
  [cat >&2 << 'EOF'
ERROR: --enable-xml was selected, but some required header files were
not found.  Please see the notes in the README file regarding xsdcxx
and libxerces-c.
EOF
exit 1
])
AC_LANG_POP

fi


#############################################
# Checks for libraries.
AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([tiff], [TIFFOpen], [], 
  [cat >&2 << 'EOF'
ERROR: The required library libtiff was not found.  Please see the notes
in the README file regarding libtiff.
EOF
exit 1
])



#############################################
# Checks for header files.
AC_HEADER_STDC

# Note to maintainer: fold this long line with fmt -w 78, then add \
AC_CHECK_HEADERS([OS.h inttypes.h libintl.h limits.h locale.h stddef.h \
stdint.h stdlib.h string.h sys/param.h unistd.h wchar.h float.h])



#############################################
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
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])



#############################################
# Checks for library functions.

AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FSEEKO
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_OBSTACK
AC_FUNC_STRERROR_R
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_FUNC_VPRINTF

# Note to maintainer: fold this long line with fmt -w 78, then add \
AC_CHECK_FUNCS([dup2 fchdir ftruncate getcwd getpagesize lchown mempcpy \
memset putenv setenv sqrt stpcpy strchr strcspn strdup strerror strndup \
strpbrk strrchr strspn strstr strverscmp wcwidth rpmatch floor getdelim \
memmove strtol strtoul realloc euidaccess gethostname getusershell \
stdtoull strtoull])



#############################################
# Output

AM_CONDITIONAL([BUILD_PYTHON], [test "x${enable_python}" = "xyes"])
AM_CONDITIONAL([BUILD_XML], [test "x${enable_xml}" = "xyes"])

DX_DOXYGEN_FEATURE(ON)
DX_HTML_FEATURE(ON)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)

DX_INIT_DOXYGEN([starrynite], [Doxyfile], [doxygen-doc])

gl_INIT

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([lib/Makefile])
AC_CONFIG_FILES([lib/gnulib/Makefile])
AC_CONFIG_FILES([lib/starrynite/Makefile])
AC_CONFIG_FILES([lib/starrynite/tests/Makefile])
AC_CONFIG_FILES([lib/starrynite-xml/Makefile])
AC_CONFIG_FILES([lib/starrynite-py/Makefile])
AC_CONFIG_FILES([bin/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/gnulib/Makefile])
AC_CONFIG_FILES([tests/starrynite-xml/Makefile])
AC_CONFIG_FILES([tests/starrynite-py/Makefile])
AC_CONFIG_FILES([Doxyfile])

AC_OUTPUT
