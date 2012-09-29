//==============================================================================
// Compiler mathematical functions
//==============================================================================

#ifndef COMPILERMATH_H
#define COMPILERMATH_H

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

extern "C" double COMPILER_EXPORT factorial(double pNb);

extern "C" double COMPILER_EXPORT arbitrary_log(double pNb, double pBase);

extern "C" double COMPILER_EXPORT gcd_multi(int pCount, ...);
extern "C" double COMPILER_EXPORT lcm_multi(int pCount, ...);
extern "C" double COMPILER_EXPORT multi_max(int pCount, ...);
extern "C" double COMPILER_EXPORT multi_min(int pCount, ...);

#ifdef Q_WS_WIN
    extern "C" double COMPILER_EXPORT asinh(double pNb);
    extern "C" double COMPILER_EXPORT acosh(double pNb);
    extern "C" double COMPILER_EXPORT atanh(double pNb);
#endif

extern "C" void COMPILER_EXPORT do_nonlinearsolve(void (*pFunction)(double *, double *, void *),
                                                  double *pParams, int *pRes, int pSize, void *pUserData);

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
