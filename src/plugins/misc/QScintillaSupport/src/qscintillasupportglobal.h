#ifndef QSCINTILLASUPPORTGLOBAL_H
#define QSCINTILLASUPPORTGLOBAL_H

#ifdef _WIN32
    #ifdef QScintillaSupport_PLUGIN
        #define QSCINTILLASUPPORT_EXPORT __declspec(dllexport)
    #else
        #define QSCINTILLASUPPORT_EXPORT __declspec(dllimport)
    #endif
#else
    #define QSCINTILLASUPPORT_EXPORT
#endif

#endif
