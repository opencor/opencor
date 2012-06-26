#ifndef CORESOLVERGLOBAL_H
#define CORESOLVERGLOBAL_H

#ifdef _WIN32
    #ifdef CoreSolver_PLUGIN
        #define CORESOLVER_EXPORT __declspec(dllexport)
    #else
        #define CORESOLVER_EXPORT __declspec(dllimport)
    #endif
#else
    #define CORESOLVER_EXPORT
#endif

#endif
