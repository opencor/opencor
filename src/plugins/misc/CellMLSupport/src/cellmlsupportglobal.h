#ifndef CELLMLSUPPORTGLOBAL_H
#define CELLMLSUPPORTGLOBAL_H

#ifdef _WIN32
    #ifdef CellMLSupport_PLUGIN
        #define CELLMLSUPPORT_EXPORT __declspec(dllexport)
    #else
        #define CELLMLSUPPORT_EXPORT __declspec(dllimport)
    #endif
#else
    #define CELLMLSUPPORT_EXPORT
#endif

#endif
