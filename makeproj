#!/bin/sh

OS=`uname -s`

if [ $OS = "Linux" ]; then
    echo "\033[44;37;1mMaking OpenCOR...\033[0m"
fi

cd build

cmake ..
make $*

cd ..

if [ $OS = "Linux" ]; then
    echo "\033[42;37;1mAll done!\033[0m"
fi
