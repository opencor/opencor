gcc -c -Iinclude src/external.cpp
gcc -dynamiclib -install_name @rpath/libexternal.1.1.dylib \
    -current_version 1.1 external.o -o lib/libexternal.1.1.dylib
echo Installing into $1
mkdir -p $1
cp -r include $1/
cp -r lib $1/
