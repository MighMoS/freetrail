#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

AC_INIT([freetrail],[0.0.6],[mighmos@mighmos.org])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([freetrail], [0.0.6])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 maps/Makefile
                 ])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG_CPLUSPLUS

# Checks for header files.
AC_CHECK_HEADERS([tr1/memory])

# Checks for libraries.
PKG_CHECK_MODULES([LIBXML], [libxml++-2.6 >= 2.10.0])
AC_SUBST(LIBXML_CFLAGS)
AC_SUBST(LIBXML_LIBS)

PKG_CHECK_MODULES([LIBCPPTEST], [libcpptest], [havecpptest=true], [havecpptest=false])
AC_SUBST(LIBCPPTEST_CFLAGS)
AC_SUBST(LIBCPPTEST_LIBS)
if test "x$havecpptest"="xtrue"; then
    AC_CONFIG_FILES([test/Makefile])
fi

AC_MSG_CHECKING(whether to build with debug information)

# create configure parameter and init variable $debugger
debugger=no
AC_ARG_ENABLE(debug,
    AS_HELP_STRING(
        [--enable-debug],
        [generate a debug build (default=no)]
    ),
    debugger="$enableval"
)

# resulting value to screen (to complete checking message)
AC_MSG_RESULT($debugger)

# set DEBUG flag and introduce additional compiler flags
if test x"$debugger" = x"yes"; then
    CXXDEBUG="-g3 -O0"
else
    CXXDEBUG="-DNDEBUG"
fi
AC_SUBST(CXXDEBUG)

# Defines so that we know where to look for our various resources (thanks, evolution)
privdatadir='${datadir}'/freetrail
AC_SUBST(privdatadir)

mapsdir='${privdatadir}'/maps
AC_SUBST(mapsdir)

testdir=test/
AC_SUBST(testdir)
AC_OUTPUT
echo [$PACKAGE_NAME will be installed to $prefix]
