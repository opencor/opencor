#ifndef CLANGGLOBAL_H
#define CLANGGLOBAL_H

#ifdef Clang_PLUGIN
    #define CLANG_EXPORT __declspec(dllexport)
#else
    #define CLANG_EXPORT __declspec(dllimport)
#endif

#endif
