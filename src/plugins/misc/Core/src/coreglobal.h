#ifndef COREGLOBAL_H
#define COREGLOBAL_H

#ifdef OpenCOR_MAIN
    #define CORE_EXPORT
#else
    #ifdef Core_PLUGIN
        #define CORE_EXPORT __declspec(dllexport)
    #else
        #define CORE_EXPORT __declspec(dllimport)
    #endif
#endif

#endif
