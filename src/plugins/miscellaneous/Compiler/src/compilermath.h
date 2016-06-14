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

#pragma once

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

extern "C" double compiler_fabs(double pNb);

extern "C" double compiler_log(double pNb);
extern "C" double compiler_exp(double pNb);

extern "C" double compiler_floor(double pNb);
extern "C" double compiler_ceil(double pNb);

extern "C" double compiler_factorial(double pNb);

extern "C" double compiler_sin(double pNb);
extern "C" double compiler_sinh(double pNb);
extern "C" double compiler_asin(double pNb);
extern "C" double compiler_asinh(double pNb);

extern "C" double compiler_cos(double pNb);
extern "C" double compiler_cosh(double pNb);
extern "C" double compiler_acos(double pNb);
extern "C" double compiler_acosh(double pNb);

extern "C" double compiler_tan(double pNb);
extern "C" double compiler_tanh(double pNb);
extern "C" double compiler_atan(double pNb);
extern "C" double compiler_atanh(double pNb);

extern "C" double compiler_sec(double pNb);
extern "C" double compiler_sech(double pNb);
extern "C" double compiler_asec(double pNb);
extern "C" double compiler_asech(double pNb);

extern "C" double compiler_csc(double pNb);
extern "C" double compiler_csch(double pNb);
extern "C" double compiler_acsc(double pNb);
extern "C" double compiler_acsch(double pNb);

extern "C" double compiler_cot(double pNb);
extern "C" double compiler_coth(double pNb);
extern "C" double compiler_acot(double pNb);
extern "C" double compiler_acoth(double pNb);

extern "C" double compiler_arbitrary_log(double pNb, double pBase);

extern "C" double compiler_pow(double pNb1, double pNb2);

extern "C" double compiler_multi_min(int pCount, ...);
extern "C" double compiler_multi_max(int pCount, ...);

extern "C" double compiler_gcd_multi(int pCount, ...);
extern "C" double compiler_lcm_multi(int pCount, ...);

//==============================================================================
// End of file
//==============================================================================
