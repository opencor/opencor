/**
 * @file    common.h
 * @brief   Common functions and macros for C and C++.
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

#ifndef LIBSEDML_COMMON_H
#define LIBSEDML_COMMON_H 1


#include <sedml/common/libsedml-config.h>


#include <stdio.h>


#if STDC_HEADERS
#  include <errno.h>
#  include <float.h>
#  include <stdarg.h>
#  include <stdlib.h>
#  include <string.h>
#endif

#if HAVE_MATH_H
#  include <math.h>
#endif

#if HAVE_IEEFP_H
#  include <ieeefp.h>
#endif

#ifndef errno
extern int errno;
#endif


/**
 * Default format used by libSEDML when writing out floating-point numbers
 * into the XML produced by libSEDML.  This is used by
 * StringBuffer_appendReal.
 */
#define LIBSEDML_FLOAT_FORMAT "%.15g"

#define LIBSEDML_DOUBLE_PRECISION 15

static const int SEDML_INT_MAX = 2147483647;
static const int SEDML_INT_MIN = -2147483647 - 1;

/*
 * Sometimes the line/column numbers reported by the underlying parsers are
 * simply invalid.  This especially occurs in error situations when the
 * error is severe.  Typically you're not supposed to call the parsers'
 * getline()/getcolumn() functions in those situations, but it's hard to
 * avoid because of the flow of control in our code.  The problem is, in
 * some cases, calling the parser's getline()/getcolumn() functions results
 * in a segmentation fault.  (Case in point: this was happening using
 * Xerces 2.8 on 32-bit systems.)  We need to communicate that a
 * line/column number value is actually invalid or meaningless.
 *
 * Doing that presents a new problem: our line/column number values are
 * unsigned integers, which thus only allows 2 states to be
 * communicated. (Explanation: you could use 0 to communicate an invalid
 * state and let all other values indicate a valid state.  If we had signed
 * integers, we could communicate 3 state values, using negative, 0, and
 * positive values for the different states.)  Unfortunately we can't use 0
 * because that value is already being used to communicate a different
 * meaning between users of XMLError objects.
 *
 * The following is a bit of a hack, but not completely unrealistic: we use
 * the largest possible value of the data type used to represent
 * line/column numbers.  The probability that an error in an file will
 * involve these specific numbers is vanishingly small.  Thus, we can say
 * that if user code encounters these values as line/column numbers, they
 * should assume they are actually meaningless and no valid line/column
 * number could be provided by the parser.
 */

#define LIBSEDML_UNKNOWN_LINE SEDML_INT_MAX
#define LIBSEDML_UNKNOWN_COLUMN SEDML_INT_MAX


#include <sedml/common/extern.h>
#include <sbml/util/memory.h>
#include <sbml/util/util.h>
#include <sedml/common/operationReturnValues.h>


#endif  /* LIBSEDML_COMMON_H */
