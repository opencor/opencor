/**
 * @file    SedTypeCodes.h
 * @brief   Enumeration to identify Sed objects at runtime
 *
 * <!--------------------------------------------------------------------------
 *
 * This file is part of libSEDML.  Please visit http://sed-ml.org for more
 * information about SED-ML. The latest version of libSEDML can be found on
 * github: https://github.com/fbergmann/libSEDML/
 *
 *
 * Copyright (c) 2013-2014, Frank T. Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---------------------------------------------------------------------- -->
 */

#ifndef SedTypeCodes_h
#define SedTypeCodes_h


#include <sedml/common/libsedml-config.h>
#include <sedml/common/extern.h>

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * An enumeration of Sed types to help identify Sed objects at runtime.
 * Abstract types do not have a typecode since they cannot be instantiated.
 *
 * (NOTES)
 *
 *  - Each typecode is used as a return value (int) of the following functions
 *
 *     - virtual int SedBase::getTypeCode() const;
 *     - virtual int ListOf::getItemTypeCode() const;
 *
 *    (In libSed 5, the type of return values in these functions have been changed
 *     from typecode (int) to int for extensibility.)
 *
 *  - Each pacakge extension must define similar enum type for each SedBase subclass
 *    (e.g. SedLayoutTypeCode_t for the layout extension, SedGroupTypeCode_t for
 *          group extension).
 *
 *  - The value of each typecode can be duplicated between those of different
 *    packages.
 *
 *  - To distinguish the typecodes of different packages, not only the return
 *    value of getTypeCode() but also that of getPackageName() must be checked
 *    as follows:
 *
 *       void example (const SedBase *sb)
 *       {
 *         cons std::string pkgName = sb->getPackageName();
 *         if (pkgName == "core")
 *         {
 *           switch (sb->getTypeCode())
 *           {
 *             case SEDML_MODEL:
 *                ....
 *                break;
 *             case SEDML_REACTION:
 *                ....
 *           }
 *         }
 *         else if (pkgName == "layout")
 *         {
 *           switch (sb->getTypeCode())
 *           {
 *             case SEDML_LAYOUT_LAYOUT:
 *                ....
 *                break;
 *             case SEDML_LAYOUT_REACTIONGLYPH:
 *                ....
 *           }
 *         }
 *         ...
 *       }
 *
 */
typedef enum
{
  SEDML_UNKNOWN                        =  0
  , SEDML_DOCUMENT                       =  1
  , SEDML_MODEL                          =  20
  , SEDML_CHANGE                         =  21
  , SEDML_CHANGE_ATTRIBUTE               =  22
  , SEDML_CHANGE_REMOVEXML               =  23
  , SEDML_CHANGE_COMPUTECHANGE           =  24
  , SEDML_CHANGE_ADDXML                  =  25
  , SEDML_CHANGE_CHANGEXML               =  26
  , SEDML_DATAGENERATOR                  =  30
  , SEDML_VARIABLE                       =  31
  , SEDML_PARAMETER                      =  32
  , SEDML_TASK                           =  40
  , SEDML_TASK_SUBTASK                   =  41
  , SEDML_TASK_SETVALUE                  =  42
  , SEDML_TASK_REPEATEDTASK              =  43
  , SEDML_OUTPUT                         =  50
  , SEDML_OUTPUT_DATASET                 =  51
  , SEDML_OUTPUT_CURVE                   =  52
  , SEDML_OUTPUT_SURFACE                 =  53
  , SEDML_OUTPUT_REPORT                  =  54
  , SEDML_OUTPUT_PLOT2D                  =  55
  , SEDML_OUTPUT_PLOT3D                  =  56
  , SEDML_SIMULATION                     =  60
  , SEDML_SIMULATION_ALGORITHM           =  61
  , SEDML_SIMULATION_UNIFORMTIMECOURSE   =  62
  , SEDML_SIMULATION_ALGORITHM_PARAMETER = 63
  , SEDML_SIMULATION_ONESTEP             =  64
  , SEDML_SIMULATION_STEADYSTATE         =  65
  , SEDML_RANGE                          = 70
  , SEDML_RANGE_UNIFORMRANGE             = 71
  , SEDML_RANGE_VECTORRANGE              = 72
  , SEDML_RANGE_FUNCTIONALRANGE          = 73
  , SEDML_LIST_OF                        =  100

} SedTypeCode_t;



/**
 * This method takes an Sed type code and returns a string representing
 * the code.
 *
 * @if clike LibSed attaches an identifying code to every kind of Sed
 * object.  These are known as <em>Sed type codes</em>.  The set of
 * possible type codes is defined in the enumeration #SedTypeCode_t.
 * The names of the type codes all begin with the characters @c
 * SEDML_. @endif@if java LibSed attaches an identifying code to every
 * kind of Sed object.  These are known as <em>Sed type codes</em>.  In
 * other languages, the set of type codes is stored in an enumeration; in
 * the Java language interface for libSed, the type codes are defined as
 * static integer constants in the interface class {@link
 * libsbmlConstants}.  The names of the type codes all begin with the
 * characters @c SEDML_. @endif@if python LibSed attaches an identifying
 * code to every kind of Sed object.  These are known as <em>Sed type
 * codes</em>.  In the Python language interface for libSed, the type
 * codes are defined as static integer constants in the interface class
 * @link libsbml@endlink.  The names of the type codes all begin with the
 * characters @c SEDML_. @endif@if csharp LibSed attaches an identifying
 * code to every kind of Sed object.  These are known as <em>Sed type
 * codes</em>.  In the C# language interface for libSed, the type codes
 * are defined as static integer constants in the interface class @link
 * libsbml@endlink.  The names of the type codes all begin with
 * the characters @c SEDML_. @endif@~
 *
 * @return a human readable name for the given
 * @if clike #SedTypeCode_t value@else Sed type code@endif.
 *
 * @note The caller does not own the returned string and is therefore not
 * allowed to modify it.
 */
LIBSEDML_EXTERN
const char *
SedTypeCode_toString(int tc);


END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /* SedTypeCodes_h */
