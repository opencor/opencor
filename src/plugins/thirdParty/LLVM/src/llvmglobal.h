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
// LLVM global
//==============================================================================

#pragma once

//==============================================================================

#ifdef _WIN32
    #ifdef LLVM_PLUGIN
        #define LLVM_EXPORT __declspec(dllexport)
    #else
        #define LLVM_EXPORT __declspec(dllimport)
    #endif
#else
    #define LLVM_EXPORT
#endif

//==============================================================================

namespace llvm {

//==============================================================================

class LLVM_EXPORT NonCopyable
{
    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        const NonCopyable& operator=(const NonCopyable&) = delete;
};

//==============================================================================

}   // namespace llvm

//==============================================================================
// End of file
//==============================================================================
