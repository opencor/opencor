//==============================================================================
// Computer mathematical functions
//==============================================================================

#include "computermath.h"

//==============================================================================

#include <cmath>
#include <cstdarg>
#include <cstdlib>

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
    return std::log(pNb)/std::log(pBase);
}

//==============================================================================

double quot(double pNb1, double pNb2)
{
    return (pNb1-fmod(pNb1, pNb2))/pNb2;
}

//==============================================================================

double gcdPair(double pNb1, double pNb2)
{
    #define EVEN(pNb) !(pNb & 1)

    int nb1 = std::fabs(pNb1);
    int nb2 = std::fabs(pNb2);

    if (!nb1)
        return nb2;

    if (!nb2)
        return nb1;

    int shift = 0;

    while (EVEN(nb1) && EVEN(nb2)) {
        ++shift;

        nb1 >>= 1;
        nb2 >>= 1;
    }

    do {
        if (EVEN(nb1))
          nb1 >>= 1;
        else if (EVEN(nb2))
            nb2 >>= 1;
        else if (nb1 >= nb2)
            nb1 = (nb1-nb2) >> 1;
        else
            nb2 = (nb2-nb1) >> 1;
    } while (nb1);

    return nb2 << shift;
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

#ifdef Q_WS_WIN
double asinh(double pNb)
{
    return log(pNb+sqrt(pNb*pNb+1));
}
#endif

//==============================================================================

#ifdef Q_WS_WIN
double acosh(double pNb)
{
    return log(pNb+sqrt(pNb*pNb-1));
}
#endif

//==============================================================================

#ifdef Q_WS_WIN
double atanh(double pNb)
{
    return 0.5*(log(1+pNb)-log(1-pNb));
}
#endif

//==============================================================================

double piecewise(double pCondition, double pTrue, double pFalse)
{
    return pCondition?pTrue:pFalse;
}

//==============================================================================
// End of file
//==============================================================================
