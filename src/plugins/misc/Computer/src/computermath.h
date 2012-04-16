//==============================================================================
// Computer mathematical functions
//==============================================================================

#ifndef COMPUTERMATH_H
#define COMPUTERMATH_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

extern "C" double COMPUTER_EXPORT _not(double pNb);
extern "C" double COMPUTER_EXPORT _or(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT _xor(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT _and(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT eq(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT neq(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT lt(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT gt(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT le(double pNb1, double pNb2);
extern "C" double COMPUTER_EXPORT ge(double pNb1, double pNb2);

extern "C" double COMPUTER_EXPORT factorial(double pNb);

extern "C" double COMPUTER_EXPORT arbitraryLog(double pNb, double pBase);
extern "C" double COMPUTER_EXPORT quot(double pNb1, double pNb2);

extern "C" double COMPUTER_EXPORT gcd(int pCount, ...);
extern "C" double COMPUTER_EXPORT lcm(int pCount, ...);
extern "C" double COMPUTER_EXPORT max(int pCount, ...);
extern "C" double COMPUTER_EXPORT min(int pCount, ...);

#ifdef Q_WS_WIN
    extern "C" double COMPUTER_EXPORT asinh(double pNb);
    extern "C" double COMPUTER_EXPORT acosh(double pNb);
    extern "C" double COMPUTER_EXPORT atanh(double pNb);
#endif

extern "C" double COMPUTER_EXPORT piecewise(double pCondition, double pTrue, double pFalse);

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
