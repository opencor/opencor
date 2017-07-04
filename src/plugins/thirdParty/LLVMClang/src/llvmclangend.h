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
// LLVMClang end
//==============================================================================

#if defined(Q_OS_WIN)
    #pragma warning(pop)
#elif defined(Q_OS_LINUX)
    #pragma GCC diagnostic error "-Wmissing-field-initializers"
    #pragma GCC diagnostic error "-Wstrict-aliasing"
    #pragma GCC diagnostic error "-Wunused-parameter"
#elif defined(Q_OS_MAC)
    #pragma GCC diagnostic error "-Wunused-parameter"
#else
    #error Unsupported platform
#endif

//==============================================================================
// End of file
//==============================================================================
