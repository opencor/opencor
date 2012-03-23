#ifndef VIEWERGLOBAL_H
#define VIEWERGLOBAL_H

#include <QtGlobal>

#ifdef Viewer_PLUGIN
    #define VIEWER_EXPORT Q_DECL_EXPORT
#else
    #define VIEWER_EXPORT Q_DECL_IMPORT
#endif

#endif
