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
// LLVMClang begin
//==============================================================================

#if defined(Q_OS_WIN)
    #pragma warning(push)
    #pragma warning(disable: 4141)
    #pragma warning(disable: 4146)
    #pragma warning(disable: 4267)
    #pragma warning(disable: 4291)
    #pragma warning(disable: 4624)
#elif defined(Q_OS_LINUX)
    #pragma GCC diagnostic push
    #if defined(__GNUC__) && (__GNUC__ >= 8)
        #pragma GCC diagnostic ignored "-Wclass-memaccess"
    #endif
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wstrict-aliasing"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wcast-align"
    #pragma clang diagnostic ignored "-Wcomma"
    #pragma clang diagnostic ignored "-Wconversion"
    #pragma clang diagnostic ignored "-Wdeprecated"
    #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
    #pragma clang diagnostic ignored "-Wduplicate-enum"
    #pragma clang diagnostic ignored "-Wextra-semi"
    #pragma clang diagnostic ignored "-Wimplicit-int-conversion"
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    #pragma clang diagnostic ignored "-Wmissing-variable-declarations"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wshadow"
    #pragma clang diagnostic ignored "-Wshadow-field"
    #pragma clang diagnostic ignored "-Wshadow-field-in-constructor"
    #pragma clang diagnostic ignored "-Wshorten-64-to-32"
    #pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
    #pragma clang diagnostic ignored "-Wswitch-enum"
    #pragma clang diagnostic ignored "-Wundef"
    #pragma clang diagnostic ignored "-Wundefined-reinterpret-cast"
    #pragma clang diagnostic ignored "-Wunused-parameter"
    #pragma clang diagnostic ignored "-Wunused-template"
    #pragma clang diagnostic ignored "-Wweak-vtables"
#endif

//==============================================================================
// End of file
//==============================================================================
