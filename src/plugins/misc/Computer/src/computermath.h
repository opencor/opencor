//==============================================================================
// Computer mathematical functions
//==============================================================================

#ifndef COMPUTERMATH_H
#define COMPUTERMATH_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

extern "C" double _not(double pNb);
extern "C" double _or(double pNb1, double pNb2);
extern "C" double _xor(double pNb1, double pNb2);
extern "C" double _and(double pNb1, double pNb2);
extern "C" double eq(double pNb1, double pNb2);
extern "C" double neq(double pNb1, double pNb2);
extern "C" double lt(double pNb1, double pNb2);
extern "C" double gt(double pNb1, double pNb2);
extern "C" double le(double pNb1, double pNb2);
extern "C" double ge(double pNb1, double pNb2);

extern "C" double factorial(double pNb);

extern "C" double arbitraryLog(double pNb, double pBase);
extern "C" double quot(double pNb1, double pNb2);
extern "C" double rem(double pNb1, double pNb2);

extern "C" double gcd(int pCount, ...);
extern "C" double lcm(int pCount, ...);
extern "C" double max(int pCount, ...);
extern "C" double min(int pCount, ...);

extern "C" double piecewise(double pCondition, double pTrue, double pFalse);

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
