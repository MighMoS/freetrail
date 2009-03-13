#!/bin/sh
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

ORIGDIR=`pwd`
cd "$srcdir"
PROJECT=Freetrail

autoreconf -fi -I m4
cd "$ORIGDIR" || exit $?

if test -z "$AUTOGEN_SUBDIR_MODE"; then
	echo Running $srcdir/configure "$@"
        $srcdir/configure "$@"

        echo 
        echo "Now type 'make' to compile $PROJECT."
fi

