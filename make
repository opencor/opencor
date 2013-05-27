#!/bin/sh

echo "\033[44;37;1mMaking OpenCOR...\033[0m"

cd build

cmake ..

exitCode=$?

if [ $exitCode -ne 0 ]; then
    cd ..

    exit $exitCode
fi

make $*

exitCode=$?

cd ..

if [ $exitCode -eq 0 ]; then
    echo "\033[42;37;1mAll done!\033[0m"
fi

exit $exitCode
