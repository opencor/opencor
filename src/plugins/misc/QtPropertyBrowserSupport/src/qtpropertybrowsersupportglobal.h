#ifndef QTPROPERTYBROWSERSUPPORTGLOBAL_H
#define QTPROPERTYBROWSERSUPPORTGLOBAL_H

#ifdef _WIN32
    #ifdef QtPropertyBrowserSupport_PLUGIN
        #define QTPROPERTYBROWSERSUPPORT_EXPORT __declspec(dllexport)
    #else
        #define QTPROPERTYBROWSERSUPPORT_EXPORT __declspec(dllimport)
    #endif
#else
    #define QTPROPERTYBROWSERSUPPORT_EXPORT
#endif

#endif
