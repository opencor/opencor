//==============================================================================
// Computer mathematical functions
//==============================================================================

#include "computermath.h"

//==============================================================================

#include <math.h>

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

double factorOf(double pNb1, double pNb2)
{
    return !((int) pNb1 % (int) pNb2);
}

//==============================================================================

double quotient(double pNb1, double pNb2)
{
    return pNb2?(int) pNb1/(int) pNb2:pNb1/0.0;
}

//==============================================================================

double rem(double pNb1, double pNb2)
{
    return pNb2?(int) pNb1 % (int) pNb2:pNb1/0.0;
}

//==============================================================================

double xOr(double pNb1, double pNb2)
{
    return (pNb1 != 0)^(pNb2 != 0);
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
    double *storage1, *storage2, *tempStorage;
    int i, iMax, j = 0;

    storage1 = new double[pCount];
    storage2 = new double[pCount >> 1];

    va_start(parameters, pCount);
        for (i = 0, iMax = pCount-1; i < iMax; i += 2) {
            double param1, param2;

            param1 = va_arg(parameters, double);
            param2 = va_arg(parameters, double);

            storage1[j++] = gcdPair(param1, param2);
        }

        if (i < pCount)
            storage1[j++] = va_arg(parameters, double);
    va_end(parameters);

    while (j != 1) {
        pCount = j;
        j = 0;

        for (i = 0, iMax = j-1; i < iMax; i += 2)
            storage2[j++] = gcdPair(storage1[i], storage1[i+1]);

        if (i < j)
            storage2[j++] = storage1[i];

        tempStorage = storage1;

        storage1 = storage2;
        storage2 = tempStorage;
    }

    double res = storage1[0];

    delete[] storage1;
    delete[] storage2;

    return res;
}

//==============================================================================

double lcm(int pCount, ...)
{
    if (!pCount)
        return 1.0;

    va_list parameters;
    double *storage1, *storage2, *tempStorage;
    int i, iMax, j = 0;

    storage1 = new double[pCount];
    storage2 = new double[pCount >> 1];

    va_start(parameters, pCount);
        for (i = 0, iMax = pCount-1; i < iMax; i += 2) {
            double param1, param2;

            param1 = va_arg(parameters, double);
            param2 = va_arg(parameters, double);

            storage1[j++] = lcmPair(param1, param2);
        }

        if (i < pCount)
            storage1[j++] = va_arg(parameters, double);
    va_end(parameters);

    while (j != 1) {
        pCount = j;
        j = 0;

        for (i = 0, iMax = j-1; i < iMax; i += 2)
            storage2[j++] = lcmPair(storage1[i], storage1[i+1]);

        if (i < j)
            storage2[j++] = storage1[i];

        tempStorage = storage1;

        storage1 = storage2;
        storage2 = tempStorage;
    }

    double res = storage1[0];

    delete[] storage1;
    delete[] storage2;

    return res;
}

//==============================================================================

double max(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", NULL);

    va_list parameters;
    double best, attempt;

    va_start(parameters, pCount);
        best = va_arg(parameters, double);

        while (--pCount) {
            attempt = va_arg(parameters, double);

            if (attempt > best)
                best = attempt;
        }
    va_end(parameters);

    return best;
}

//==============================================================================

double min(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", NULL);

    va_list parameters;
    double best, attempt;

    va_start(parameters, pCount);
        best = va_arg(parameters, double);

        while (--pCount) {
            attempt = va_arg(parameters, double);

            if (attempt < best)
                best = attempt;
        }
    va_end(parameters);

    return best;
}

//==============================================================================
// End of file
//==============================================================================
