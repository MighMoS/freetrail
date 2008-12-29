#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

AC_INIT([freetrail], [0.0.1], [mighmos@mighmos.org])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([freetrail], [0.0.1])

AC_CONFIG_SRCDIR([src/Makefile.am])
AC_CONFIG_FILES([Makefile
                 src/Makefile])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.
PKG_CHECK_MODULES([LIBXML], [libxml++-2.6 >= 2.10.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_C_RESTRICT

# Checks for library functions.

AC_OUTPUT
echo [$PACKAGE_NAME will be installed to $prefix]
