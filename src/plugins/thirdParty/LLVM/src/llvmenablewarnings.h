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
// LLVM enable warnings
//==============================================================================

#if defined(LLVMENABLEWARNINGS_H) && !defined(LLVMDISABLEWARNINGS_H)
#define LLVMENABLEWARNINGS_H

//==============================================================================

#if defined(Q_OS_WIN)
    #pragma warning(pop)
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    #pragma GCC diagnostic error "-Wunused-parameter"
    #pragma GCC diagnostic error "-Wstrict-aliasing"
#else
    #error Unsupported platform
#endif

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
