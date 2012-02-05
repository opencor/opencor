//==============================================================================
// Computer mathematical functions
//==============================================================================

#ifndef COMPUTERMATH_H
#define COMPUTERMATH_H

//==============================================================================

#include "computerglobal.h"

//==============================================================================

extern "C" double factorial(double pNb);

extern "C" double arbitraryLog(double pNb, double pBase);
extern "C" double factorOf(double pNb1, double pNb2);
extern "C" double quotient(double pNb1, double pNb2);
extern "C" double rem(double pNb1, double pNb2);
extern "C" double xOr(double pNb1, double pNb2);

extern "C" double gcd(int pCount, ...);
extern "C" double lcm(int pCount, ...);
extern "C" double max(int pCount, ...);
extern "C" double min(int pCount, ...);

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
