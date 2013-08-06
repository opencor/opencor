#ifndef CORECELLMLEDITINGGLOBAL_H
#define CORECELLMLEDITINGGLOBAL_H

#ifdef _WIN32
    #ifdef CoreCellMLEditing_PLUGIN
        #define CORECELLMLEDITING_EXPORT __declspec(dllexport)
    #else
        #define CORECELLMLEDITING_EXPORT __declspec(dllimport)
    #endif
#else
    #define CORECELLMLEDITING_EXPORT
#endif

#endif
