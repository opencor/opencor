#ifndef VIEWERGLOBAL_H
#define VIEWERGLOBAL_H

#ifdef _WIN32
    #ifdef Viewer_PLUGIN
        #define VIEWER_EXPORT __declspec(dllexport)
    #else
        #define VIEWER_EXPORT __declspec(dllimport)
    #endif
#else
    #define VIEWER_EXPORT
#endif

#endif
