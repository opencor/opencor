#ifndef COMPUTERGLOBAL_H
#define COMPUTERGLOBAL_H

#include <QtGlobal>

#ifdef Computer_PLUGIN
    #define COMPUTER_EXPORT Q_DECL_EXPORT
#else
    #define COMPUTER_EXPORT Q_DECL_IMPORT
#endif

#endif
