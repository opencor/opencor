#ifndef VIEWERGLOBAL_H
#define VIEWERGLOBAL_H

#ifdef Viewer_PLUGIN
    #define VIEWER_EXPORT __declspec(dllexport)
#else
    #define VIEWER_EXPORT __declspec(dllimport)
#endif

#endif
