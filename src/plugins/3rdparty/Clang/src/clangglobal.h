#ifndef CLANGGLOBAL_H
#define CLANGGLOBAL_H

#include <QtGlobal>

#ifdef Clang_PLUGIN
    #define CLANG_EXPORT Q_DECL_EXPORT
#else
    #define CLANG_EXPORT Q_DECL_IMPORT
#endif

#endif
