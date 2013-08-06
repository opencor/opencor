#ifndef COREEDITINGGLOBAL_H
#define COREEDITINGGLOBAL_H

#ifdef _WIN32
    #ifdef CoreEditing_PLUGIN
        #define COREEDITING_EXPORT __declspec(dllexport)
    #else
        #define COREEDITING_EXPORT __declspec(dllimport)
    #endif
#else
    #define COREEDITING_EXPORT
#endif

#endif
