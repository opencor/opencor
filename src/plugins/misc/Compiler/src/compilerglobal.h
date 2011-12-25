#ifndef COMPILERGLOBAL_H
#define COMPILERGLOBAL_H

#ifdef OpenCOR_MAIN
    #define COMPILER_EXPORT
#else
    #ifdef Compiler_PLUGIN
        #define COMPILER_EXPORT Q_DECL_EXPORT
    #else
        #define COMPILER_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif
