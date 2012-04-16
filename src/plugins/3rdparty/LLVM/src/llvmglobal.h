#ifndef LLVMGLOBAL_H
#define LLVMGLOBAL_H

#include <QtGlobal>

#ifdef LLVM_PLUGIN
    #define LLVM_EXPORT Q_DECL_EXPORT
#else
    #define LLVM_EXPORT Q_DECL_IMPORT
#endif

#endif
