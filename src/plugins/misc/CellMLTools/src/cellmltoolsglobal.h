#ifndef CELLMLTOOLSGLOBAL_H
#define CELLMLTOOLSGLOBAL_H

#ifdef _WIN32
    #ifdef CellMLTools_PLUGIN
        #define CELLMLTOOLS_EXPORT __declspec(dllexport)
    #else
        #define CELLMLTOOLS_EXPORT __declspec(dllimport)
    #endif
#else
    #define CELLMLTOOLS_EXPORT
#endif

#endif
