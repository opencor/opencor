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
// LLVM begin
//==============================================================================

#if defined(Q_OS_WIN)
    #pragma warning(push)
    #pragma warning(disable: 4146)
    #pragma warning(disable: 4624)
#elif defined(Q_OS_LINUX)
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#elif defined(Q_OS_MAC)
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #error Unsupported platform
#endif

//==============================================================================
// End of file
//==============================================================================
