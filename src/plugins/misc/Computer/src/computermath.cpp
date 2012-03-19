//==============================================================================
// Computer mathematical functions
//==============================================================================

#include "computermath.h"

//==============================================================================

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

//==============================================================================

double _not(double pNb)
{
    return !pNb;
}

//==============================================================================

double _or(double pNb1, double pNb2)
{
    return pNb1 || pNb2;
}

//==============================================================================

double _xor(double pNb1, double pNb2)
{
    return (pNb1 != 0) ^ (pNb2 != 0);
}

//==============================================================================

double _and(double pNb1, double pNb2)
{
    return pNb1 && pNb2;
}

//==============================================================================

double eq(double pNb1, double pNb2)
{
    return pNb1 == pNb2;
}

//==============================================================================

double neq(double pNb1, double pNb2)
{
    return pNb1 != pNb2;
}

//==============================================================================

double lt(double pNb1, double pNb2)
{
    return pNb1 < pNb2;
}

//==============================================================================

double gt(double pNb1, double pNb2)
{
    return pNb1 > pNb2;
}

//==============================================================================

double le(double pNb1, double pNb2)
{
    return pNb1 <= pNb2;
}

//==============================================================================

double ge(double pNb1, double pNb2)
{
    return pNb1 >= pNb2;
}

//==============================================================================

double factorial(double pNb)
{
    double res = 1.0;

    while (pNb > 1.0)
        res *= pNb--;

    return res;
}

//==============================================================================

double arbitraryLog(double pNb, double pBase)
{
    return log(pNb)/log(pBase);
}

//==============================================================================

static double zero = 0.0;

//==============================================================================

double quot(double pNb1, double pNb2)
{
    return pNb2?(int) pNb1/(int) pNb2:pNb1/zero;
}

//==============================================================================

double rem(double pNb1, double pNb2)
{
    return pNb2?(int) pNb1 % (int) pNb2:pNb1/zero;
}

//==============================================================================

double gcdPair(double pNb1, double pNb2)
{
    #define EVEN(pNb) !(pNb & 1)

    uint32_t intNb1 = fabs(pNb1);
    uint32_t intNb2 = fabs(pNb2);

    if (!intNb1)
        return intNb2;

    if (!intNb2)
        return intNb1;

    int shift = 0;

    while (EVEN(intNb1) && EVEN(intNb2)) {
        ++shift;

        intNb1 >>= 1;
        intNb2 >>= 1;
    }

    do {
        if (EVEN(intNb1))
          intNb1 >>= 1;
        else if (EVEN(intNb2))
            intNb2 >>= 1;
        else if (intNb1 >= intNb2)
            intNb1 = (intNb1-intNb2) >> 1;
        else
            intNb2 = (intNb2-intNb1) >> 1;
    } while (intNb1);

    return intNb2 << shift;
}

//==============================================================================

double lcmPair(double pNb1, double pNb2)
{
    return (pNb1*pNb2)/gcdPair(pNb1, pNb2);
}

//==============================================================================

double gcd(int pCount, ...)
{
    if (!pCount)
        return 1.0;

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount)
            res = gcdPair(res, va_arg(parameters, double));
    va_end(parameters);

    return res;
}

//==============================================================================

double lcm(int pCount, ...)
{
    if (!pCount)
        return 1.0;

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount)
            res = lcmPair(res, va_arg(parameters, double));
    va_end(parameters);

    return res;
}

//==============================================================================

double max(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", NULL);

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter > res)
                res = otherParameter;
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double min(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", NULL);

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter < res)
                res = otherParameter;
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double piecewise(double pCondition, double pTrue, double pFalse)
{
    return pCondition?pTrue:pFalse;
}

//==============================================================================
// End of file
//==============================================================================
