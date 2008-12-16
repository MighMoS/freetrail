#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([freetrail], [0.0.1], [mighmos@mighmos.org])
AM_INIT_AUTOMAKE([freetrail], [0.0.1])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

PKG_CHECK_MODULES([LIBXML], [libxml++-2.6 >= 2.10.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)
                
# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
echo PACKAGE v VERSION
echo PACKAGE will be installed to $prefix
echo Type `make install` to continue
