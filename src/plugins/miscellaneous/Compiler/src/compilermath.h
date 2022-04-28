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

#pragma once

//==============================================================================

#include <QtGlobal>

//==============================================================================

extern "C" double factorial(double pNb);

extern "C" double sec(double pNb);
extern "C" double sech(double pNb);
extern "C" double asec(double pNb);
extern "C" double asech(double pNb);

extern "C" double csc(double pNb);
extern "C" double csch(double pNb);
extern "C" double acsc(double pNb);
extern "C" double acsch(double pNb);

extern "C" double cot(double pNb);
extern "C" double coth(double pNb);
extern "C" double acot(double pNb);
extern "C" double acoth(double pNb);

extern "C" double arbitrary_log(double pNb, double pBase);

extern "C" double multi_min(int pCount, ...);
extern "C" double multi_max(int pCount, ...);

extern "C" double gcd_multi(int pCount, ...);
extern "C" double lcm_multi(int pCount, ...);

//==============================================================================
// End of file
//==============================================================================
