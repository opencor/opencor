#ifndef CELLMLSUPPORTGLOBAL_H
#define CELLMLSUPPORTGLOBAL_H

#ifdef OpenCOR_MAIN
    #define CELLMLSUPPORT_EXPORT
#else
    #ifdef CellMLSupport_PLUGIN
        #define CELLMLSUPPORT_EXPORT Q_DECL_EXPORT
    #else
        #define CELLMLSUPPORT_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif
