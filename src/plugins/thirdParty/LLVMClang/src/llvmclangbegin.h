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
// LLVMClang begin
//==============================================================================

#undef emit

//==============================================================================

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(push)
    #pragma warning(disable: 4146)
    #pragma warning(disable: 4244)
    #pragma warning(disable: 4251)
    #pragma warning(disable: 4267)
    #pragma warning(disable: 4275)
    #pragma warning(disable: 4291)
    #pragma warning(disable: 4624)
    #pragma warning(disable: 4996)
#elif defined(__GNUC__) && !defined(__clang__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wredundant-move"
    #pragma GCC diagnostic ignored "-Wunused-parameter"
#else
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wbitfield-enum-conversion"
    #pragma clang diagnostic ignored "-Wcast-align"
    #pragma clang diagnostic ignored "-Wcomma"
    #pragma clang diagnostic ignored "-Wdeprecated-copy-with-dtor"
    #pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
    #pragma clang diagnostic ignored "-Wduplicate-enum"
    #pragma clang diagnostic ignored "-Wextra-semi"
    #pragma clang diagnostic ignored "-Wimplicit-int-conversion"
    #pragma clang diagnostic ignored "-Winconsistent-missing-destructor-override"
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    #pragma clang diagnostic ignored "-Wmissing-variable-declarations"
    #pragma clang diagnostic ignored "-Wold-style-cast"
    #pragma clang diagnostic ignored "-Wreserved-id-macro"
    #pragma clang diagnostic ignored "-Wshadow"
    #pragma clang diagnostic ignored "-Wshadow-field"
    #pragma clang diagnostic ignored "-Wshadow-field-in-constructor"
    #pragma clang diagnostic ignored "-Wshift-sign-overflow"
    #pragma clang diagnostic ignored "-Wshorten-64-to-32"
    #pragma clang diagnostic ignored "-Wsigned-enum-bitfield"
    #pragma clang diagnostic ignored "-Wswitch-enum"
    #pragma clang diagnostic ignored "-Wtautological-type-limit-compare"
    #pragma clang diagnostic ignored "-Wundefined-func-template"
    #pragma clang diagnostic ignored "-Wunused-parameter"
    #pragma clang diagnostic ignored "-Wweak-vtables"
#endif

//==============================================================================
// End of file
//==============================================================================
