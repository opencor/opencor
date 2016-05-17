/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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

double compiler_fabs(double pNb)
{
    return ::fabs(pNb);
}

//==============================================================================

double compiler_log(double pNb)
{
    return ::log(pNb);
}

//==============================================================================

double compiler_exp(double pNb)
{
    return ::exp(pNb);
}

//==============================================================================

double compiler_floor(double pNb)
{
    return ::floor(pNb);
}

//==============================================================================

double compiler_ceil(double pNb)
{
    return ::ceil(pNb);
}

//==============================================================================

double compiler_factorial(double pNb)
{
    double res = 1.0;

    while (pNb > 1.0)
        res *= pNb--;

    return res;
}

//==============================================================================

double compiler_sin(double pNb)
{
    return ::sin(pNb);
}

//==============================================================================

double compiler_sinh(double pNb)
{
    return ::sinh(pNb);
}

//==============================================================================

double compiler_asin(double pNb)
{
    return ::asin(pNb);
}

//==============================================================================

double compiler_asinh(double pNb)
{
    return ::asinh(pNb);
}

//==============================================================================

double compiler_cos(double pNb)
{
    return ::cos(pNb);
}

//==============================================================================

double compiler_cosh(double pNb)
{
    return ::cosh(pNb);
}

//==============================================================================

double compiler_acos(double pNb)
{
    return ::acos(pNb);
}

//==============================================================================

double compiler_acosh(double pNb)
{
    return ::acosh(pNb);
}

//==============================================================================

double compiler_tan(double pNb)
{
    return ::tan(pNb);
}

//==============================================================================

double compiler_tanh(double pNb)
{
    return ::tanh(pNb);
}

//==============================================================================

double compiler_atan(double pNb)
{
    return ::atan(pNb);
}

//==============================================================================

double compiler_atanh(double pNb)
{
    return ::atanh(pNb);
}

//==============================================================================

double compiler_sec(double pNb)
{
    return 1.0/cos(pNb);
}

//==============================================================================

double compiler_sech(double pNb)
{
    return 1.0/cosh(pNb);
}

//==============================================================================

double compiler_asec(double pNb)
{
    return acos(1.0/pNb);
}

//==============================================================================

double compiler_asech(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb-1.0));
}

//==============================================================================

double compiler_csc(double pNb)
{
    return 1.0/sin(pNb);
}

//==============================================================================

double compiler_csch(double pNb)
{
    return 1.0/sinh(pNb);
}

//==============================================================================

double compiler_acsc(double pNb)
{
    return asin(1.0/pNb);
}

//==============================================================================

double compiler_acsch(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return log(oneOverNb+sqrt(oneOverNb*oneOverNb+1.0));
}

//==============================================================================

double compiler_cot(double pNb)
{
    return 1.0/tan(pNb);
}

//==============================================================================

double compiler_coth(double pNb)
{
    return 1.0/tanh(pNb);
}

//==============================================================================

double compiler_acot(double pNb)
{
    return atan(1.0/pNb);
}

//==============================================================================

double compiler_acoth(double pNb)
{
    double oneOverNb = 1.0/pNb;

    return 0.5*log((1.0+oneOverNb)/(1.0-oneOverNb));
}

//==============================================================================

double compiler_arbitrary_log(double pNb, double pBase)
{
    return std::log(pNb)/std::log(pBase);
}

//==============================================================================

double compiler_pow(double pNb1, double pNb2)
{
    return ::pow(pNb1, pNb2);
}

//==============================================================================

double compiler_multi_min(int pCount, ...)
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

double compiler_multi_max(int pCount, ...)
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

double compiler_gcd_pair(double pNb1, double pNb2)
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

double compiler_lcm_pair(double pNb1, double pNb2)
{
    return (pNb1*pNb2)/compiler_gcd_pair(pNb1, pNb2);
}

//==============================================================================

double compiler_gcd_multi(int pCount, ...)
{
    if (!pCount)
        return 1.0;

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount)
            res = compiler_gcd_pair(res, va_arg(parameters, double));
    va_end(parameters);

    return res;
}

//==============================================================================

double compiler_lcm_multi(int pCount, ...)
{
    if (!pCount)
        return 1.0;

    va_list parameters;

    va_start(parameters, pCount);
        double res = va_arg(parameters, double);

        while (--pCount)
            res = compiler_lcm_pair(res, va_arg(parameters, double));
    va_end(parameters);

    return res;
}

//==============================================================================
// End of file
//==============================================================================
