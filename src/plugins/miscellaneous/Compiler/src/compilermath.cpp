/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Compiler mathematical functions
//==============================================================================

#include "compilermath.h"

//==============================================================================

#include <cmath>
#include <cstdarg>
#include <cstdlib>

//==============================================================================

double factorial(double pNb)
{
    double res = 1.0;

    while (pNb > 1.0)
        res *= pNb--;

    return res;
}

//==============================================================================

double arbitrary_log(double pNb, double pBase)
{
    return std::log(pNb)/std::log(pBase);
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

double gcd_multi(int pCount, ...)
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

double lcm_multi(int pCount, ...)
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

double multi_min(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", 0);

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

double multi_max(int pCount, ...)
{
    if (!pCount)
        return strtod("NAN", 0);

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
// End of file
//==============================================================================
