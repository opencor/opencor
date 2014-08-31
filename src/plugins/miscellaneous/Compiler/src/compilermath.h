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

#ifndef COMPILERMATH_H
#define COMPILERMATH_H

//==============================================================================

#include "compilerglobal.h"

//==============================================================================

#include <QtGlobal>

//==============================================================================

extern "C" double COMPILER_EXPORT factorial(double pNb);

extern "C" double COMPILER_EXPORT arbitrary_log(double pNb, double pBase);

extern "C" double COMPILER_EXPORT gcd_multi(int pCount, ...);
extern "C" double COMPILER_EXPORT lcm_multi(int pCount, ...);
extern "C" double COMPILER_EXPORT multi_max(int pCount, ...);
extern "C" double COMPILER_EXPORT multi_min(int pCount, ...);

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
