#!/bin/sh

./patchelf --remove-rpath $1
./patchelf --force-rpath --set-rpath "`echo $2 | sed 's/ORIGIN/$ORIGIN/g'`" $1 > /dev/null 2>&1
