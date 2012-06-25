#ifndef QSCINTILLASUPPORTGLOBAL_H
#define QSCINTILLASUPPORTGLOBAL_H

#ifdef QScintillaSupport_PLUGIN
    #define QSCINTILLASUPPORT_EXPORT __declspec(dllexport)
#else
    #define QSCINTILLASUPPORT_EXPORT __declspec(dllimport)
#endif

#endif
