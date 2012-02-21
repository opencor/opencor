#!/bin/sh

OS=`uname -s`

echo "\033[44;37;1mMaking OpenCOR...\033[0m"

cd build

cmake ..
make $*

cd ..

echo "\033[42;37;1mAll done!\033[0m"
