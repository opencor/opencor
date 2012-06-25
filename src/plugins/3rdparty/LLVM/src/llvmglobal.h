#ifndef LLVMGLOBAL_H
#define LLVMGLOBAL_H

#ifdef LLVM_PLUGIN
    #define LLVM_EXPORT __declspec(dllexport)
#else
    #define LLVM_EXPORT __declspec(dllimport)
#endif

#endif
