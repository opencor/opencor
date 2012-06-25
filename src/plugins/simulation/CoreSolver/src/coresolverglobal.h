#ifndef CORESOLVERGLOBAL_H
#define CORESOLVERGLOBAL_H

#ifdef CoreSolver_PLUGIN
    #define CORESOLVER_EXPORT __declspec(dllexport)
#else
    #define CORESOLVER_EXPORT __declspec(dllimport)
#endif

#endif
