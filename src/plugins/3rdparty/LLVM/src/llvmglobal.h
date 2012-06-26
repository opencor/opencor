#ifndef LLVMGLOBAL_H
#define LLVMGLOBAL_H

#ifdef _WIN32
    #ifdef LLVM_PLUGIN
        #define LLVM_EXPORT __declspec(dllexport)
    #else
        #define LLVM_EXPORT __declspec(dllimport)
    #endif
#else
    #define LLVM_EXPORT
#endif

#endif
