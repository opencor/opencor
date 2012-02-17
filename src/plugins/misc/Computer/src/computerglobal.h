#ifndef COMPUTERGLOBAL_H
#define COMPUTERGLOBAL_H

#include <QObject>

#ifdef Computer_PLUGIN
    #define COMPUTER_EXPORT Q_DECL_EXPORT
#else
    #define COMPUTER_EXPORT Q_DECL_IMPORT
#endif

#endif
