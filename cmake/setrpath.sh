#!/bin/sh

RPATH=`echo $2 | sed 's/ORIGIN/$ORIGIN/g'`

if [ "$RPATH" != "`./patchelf --print-rpath $1`" ]; then
    ./patchelf --remove-rpath $1
    ./patchelf --force-rpath --set-rpath "$RPATH" $1 > /dev/null 2>&1
fi
