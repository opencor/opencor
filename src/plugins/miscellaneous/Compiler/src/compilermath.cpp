/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Compiler mathematical functions
//==============================================================================

#include "compilermath.h"

//==============================================================================

#include <cmath>
#include <cstdarg>

//==============================================================================

double factorial(double pNb)
{
    return tgamma(pNb+1.0);
    // Note: Γ(n) = (n-1)!...
}

//==============================================================================

double sec(double pNb)
{
    return 1.0/cos(pNb);
}

//==============================================================================

double sech(double pNb)
{
    return 1.0/cosh(pNb);
}

//==============================================================================

double asec(double pNb)
{
    return acos(1.0/pNb);
}

//==============================================================================

double asech(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb-1.0));
}

//==============================================================================

double csc(double pNb)
{
    return 1.0/sin(pNb);
}

//==============================================================================

double csch(double pNb)
{
    return 1.0/sinh(pNb);
}

//==============================================================================

double acsc(double pNb)
{
    return asin(1.0/pNb);
}

//==============================================================================

double acsch(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb+1.0));
}

//==============================================================================

double cot(double pNb)
{
    return 1.0/tan(pNb);
}

//==============================================================================

double coth(double pNb)
{
    return 1.0/tanh(pNb);
}

//==============================================================================

double acot(double pNb)
{
    return atan(1.0/pNb);
}

//==============================================================================

double acoth(double pNb)
{
    static const double HALF = 0.5;

    double oneOverNb = 1.0/pNb;

    return HALF*log((1.0+oneOverNb)/(1.0-oneOverNb));
}

//==============================================================================

double arbitrary_log(double pNb, double pBase)
{
    return std::log(pNb)/std::log(pBase);
}

//==============================================================================

double multi_min(int pCount, ...)
{
    if (pCount == 0) {
        return strtod("NAN", nullptr);
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount != 0) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter < res) {
                res = otherParameter;
            }
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double multi_max(int pCount, ...)
{
    if (pCount == 0) {
        return strtod("NAN", nullptr);
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);
        double otherParameter;

        while (--pCount != 0) {
            otherParameter = va_arg(parameters, double);

            if (otherParameter > res) {
                res = otherParameter;
            }
        }
    va_end(parameters);

    return res;
}

//==============================================================================

constexpr bool isEven(uint pNb)
{
    return pNb%2 == 0;
}

//==============================================================================

double gcd_pair(double pNb1, double pNb2)
{
    auto nb1 = static_cast<unsigned int>(std::fabs(pNb1));
    auto nb2 = static_cast<unsigned int>(std::fabs(pNb2));

    if (nb1 == 0) {
        return nb2;
    }

    if (nb2 == 0) {
        return nb1;
    }

    auto mult = 1U;

    while (isEven(nb1) && isEven(nb2)) {
        mult *= 2;

        nb1 /= 2;
        nb2 /= 2;
    }

    do {
        if (isEven(nb1)) {
            nb1 /= 2;
        } else if (isEven(nb2)) {
            nb2 /= 2;
        } else if (nb1 >= nb2) {
            nb1 = (nb1-nb2)/2;
        } else {
            nb2 = (nb2-nb1)/2;
        }
    } while (nb1 != 0);

    return mult*nb2;
}

//==============================================================================

double lcm_pair(double pNb1, double pNb2)
{
    return (pNb1*pNb2)/gcd_pair(pNb1, pNb2);
}

//==============================================================================

double gcd_multi(int pCount, ...)
{
    if (pCount == 0) {
        return 1.0;
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount != 0) {
            res = gcd_pair(res, va_arg(parameters, double));
        }
    va_end(parameters);

    return res;
}

//==============================================================================

double lcm_multi(int pCount, ...)
{
    if (pCount == 0) {
        return 1.0;
    }

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount != 0) {
            res = lcm_pair(res, va_arg(parameters, double));
        }
    va_end(parameters);

    return res;
}

//==============================================================================
// End of file
//==============================================================================
