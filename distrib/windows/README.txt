The OpenSSL binaries were built from source:
 1) Download the OpenSSL source codes from https://www.openssl.org/source/
 2) Uncompress the tar.gz file
 3) From the command line:
        perl Configure VC-WIN32 no-asm
        ms\do_ms
        nmake -f ms\ntdll.mak
    or
        perl Configure VC-WIN64A no-asm
        ms\do_win64a
        nmake -f ms\ntdll.mak
