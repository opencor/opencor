#ifndef COMPILERGLOBAL_H
#define COMPILERGLOBAL_H

#ifdef _WIN32
    #ifdef Compiler_PLUGIN
        #define COMPILER_EXPORT __declspec(dllexport)
    #else
        #define COMPILER_EXPORT __declspec(dllimport)
    #endif
#else
    #define COMPILER_EXPORT
#endif

#endif
