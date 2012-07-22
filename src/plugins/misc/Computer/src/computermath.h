//==============================================================================
// Computer mathematical functions
//==============================================================================

#ifndef COMPUTERMATH_H
#define COMPUTERMATH_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

extern "C" double COMPUTER_EXPORT factorial(double pNb);

extern "C" double COMPUTER_EXPORT arbitrary_log(double pNb, double pBase);

extern "C" double COMPUTER_EXPORT gcd_multi(int pCount, ...);
extern "C" double COMPUTER_EXPORT lcm_multi(int pCount, ...);
extern "C" double COMPUTER_EXPORT multi_max(int pCount, ...);
extern "C" double COMPUTER_EXPORT multi_min(int pCount, ...);

#ifdef Q_WS_WIN
    extern "C" double COMPUTER_EXPORT asinh(double pNb);
    extern "C" double COMPUTER_EXPORT acosh(double pNb);
    extern "C" double COMPUTER_EXPORT atanh(double pNb);
#endif

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
