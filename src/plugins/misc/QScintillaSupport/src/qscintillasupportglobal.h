#ifndef QSCINTILLASUPPORTGLOBAL_H
#define QSCINTILLASUPPORTGLOBAL_H

#ifdef OpenCOR_MAIN
    #define QSCINTILLASUPPORT_EXPORT
#else
    #ifdef QScintillaSupport_PLUGIN
        #define QSCINTILLASUPPORT_EXPORT Q_DECL_EXPORT
    #else
        #define QSCINTILLASUPPORT_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif
