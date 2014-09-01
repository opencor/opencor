We build our own copy of OpenSSL, which requires the following:
 - Perl (see http://www.perl.org/); and
 - the OpenSSL source code (see https://www.openssl.org/source/).

From there, you need to:
 - Uncompress the tar.gz file
 - Do the following from the command line:
       perl Configure VC-WIN32 no-asm
       ms\do_ms
       nmake -f ms\ntdll.mak
   or
       perl Configure VC-WIN64A no-asm
       ms\do_win64a
       nmake -f ms\ntdll.mak
