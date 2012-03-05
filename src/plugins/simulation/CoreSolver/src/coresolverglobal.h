#ifndef CORESOLVERGLOBAL_H
#define CORESOLVERGLOBAL_H

#include <QtGlobal>

#ifdef CoreSolver_PLUGIN
    #define CORESOLVER_EXPORT Q_DECL_EXPORT
#else
    #define CORESOLVER_EXPORT Q_DECL_IMPORT
#endif

#endif
