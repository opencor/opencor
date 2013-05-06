#!/bin/bash

echo "\033[44;37;1mMaking OpenCOR...\033[0m"

cd build

CMAKEARGS=""
MAKEARGS=""
for ARG; do
  if [[ ${ARG:0:2} == "-D" ]];
  then
      CMAKEARGS="$CMAKEARGS $ARG";
  else
      MAKEARGS="$MAKEARGS $ARG";
  fi
done

cmake $CMAKEARGS ..

exitCode=$?

if [ $exitCode -ne 0 ]; then
    cd ..

    exit $exitCode
fi

make $MAKEARGS

exitCode=$?

cd ..

if [ $exitCode -eq 0 ]; then
    echo "\033[42;37;1mAll done!\033[0m"
fi

exit $exitCode
