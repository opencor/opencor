#ifndef CELLMLSUPPORTGLOBAL_H
#define CELLMLSUPPORTGLOBAL_H

#ifdef CellMLSupport_PLUGIN
    #define CELLMLSUPPORT_EXPORT __declspec(dllexport)
#else
    #define CELLMLSUPPORT_EXPORT __declspec(dllimport)
#endif

#endif
