#ifndef QSCINTILLASUPPORTGLOBAL_H
#define QSCINTILLASUPPORTGLOBAL_H

#include <QtGlobal>

#ifdef QScintillaSupport_PLUGIN
    #define QSCINTILLASUPPORT_EXPORT Q_DECL_EXPORT
#else
    #define QSCINTILLASUPPORT_EXPORT Q_DECL_IMPORT
#endif

#endif
