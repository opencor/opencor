/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
