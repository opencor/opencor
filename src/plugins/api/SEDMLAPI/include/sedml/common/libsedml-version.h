/**
 * @file    libsedml-version.h
 * @brief   Define libSEDML version numbers for access from client software.
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

#ifndef LIBSEDML_VERSION_H
#define LIBSEDML_VERSION_H

#include <sedml/common/extern.h>


/**
 * LIBSEDML_DOTTED_VERSION:
 *
 * A version string of the form "1.2.3".
 */
#define LIBSEDML_DOTTED_VERSION	"0.3.1"


/**
 * LIBSEDML_VERSION:
 *
 * The version as an integer: version 1.2.3 becomes 10203.  Since the major
 * number comes first, the overall number will always increase when a new
 * libSEDML is released, making it easy to use less-than and greater-than
 * comparisons when testing versions numbers.
 */
#define LIBSEDML_VERSION		301


/**
 * LIBSEDML_VERSION_STRING:
 *
 * The numeric version as a string: version 1.2.3 becomes "10203".
 */
#define LIBSEDML_VERSION_STRING	"301"


LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Returns the version number of this copy of libSEDML as an integer.
 *
 * @return the libSEDML version as an integer; version 1.2.3 becomes 10203.
 */
LIBSEDML_EXTERN
int
getLibSEDMLVersion ();


/**
 * Returns the version number of this copy of libSEDML as a string.
 *
 * @return the libSEDML version as a string; version 1.2.3 becomes
 * "1.2.3".
 *
 * @see getLibSEDMLVersionString()
 */
LIBSEDML_EXTERN
const char*
getLibSEDMLDottedVersion ();


/**
 * Returns the version number of this copy of libSEDML as a string without
 * periods.
 *
 * @return the libSEDML version as a string: version 1.2.3 becomes "10203".
 *
 * @see getLibSEDMLDottedVersion()
 */
LIBSEDML_EXTERN
const char*
getLibSEDMLVersionString ();


END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /* LIBSEDML_VERSION_H */
