/**
 * @file    libsedml-namespace.h
 * @brief   Defines C++ namespace of libSEDML
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
 *
 */

#ifndef LIBSEDML_NAMESPACE_H
#define LIBSEDML_NAMESPACE_H 1

#ifndef SWIG
#include <sbml/common/libsbml-namespace.h>

LIBSBML_CPP_NAMESPACE_USE

#endif

/*
 *
 * The idea of the following marcors are borrowed from
 * Xerces-C++ XML Parser (http://xerces.apache.org/xerces-c/).
 *
 */

/* Define to enable libSEDML C++ namespace */
#define LIBSEDML_USE_CPP_NAMESPACE 1


#if defined(__cplusplus) && defined(LIBSEDML_USE_CPP_NAMESPACE) && !defined(SWIG)
  /* C++ namespace of libSEDML */
  #define LIBSEDML_CPP_NAMESPACE            libsedml
  #define LIBSEDML_CPP_NAMESPACE_BEGIN      namespace LIBSEDML_CPP_NAMESPACE {
  #define LIBSEDML_CPP_NAMESPACE_END        }
  #define LIBSEDML_CPP_NAMESPACE_USE        using namespace LIBSEDML_CPP_NAMESPACE;
  #define LIBSEDML_CPP_NAMESPACE_QUALIFIER  LIBSEDML_CPP_NAMESPACE::

  namespace LIBSEDML_CPP_NAMESPACE {}
#else
  #define LIBSEDML_CPP_NAMESPACE
  #define LIBSEDML_CPP_NAMESPACE_BEGIN
  #define LIBSEDML_CPP_NAMESPACE_END
  #define LIBSEDML_CPP_NAMESPACE_USE
  #define LIBSEDML_CPP_NAMESPACE_QUALIFIER
#endif


#endif  /* LIBSEDML_NAMESPACE_H */

