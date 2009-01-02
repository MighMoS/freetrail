#!/bin/sh
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"
PROJECT=FreeTrail

autoreconf -fi -I m4
cd "$ORIGDIR" || exit $?

if test -z "$AUTOGEN_SUBDIR_MODE"; then
	echo Running $srcdir/configure --enable-maintainer-mode "$@"
        $srcdir/configure --enable-maintainer-mode "$@"

        echo 
        echo "Now type 'make' to compile $PROJECT."
fi

