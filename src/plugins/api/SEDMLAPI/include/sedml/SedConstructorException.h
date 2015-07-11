/**
 * @file    SedConstructorException.h
 * @brief   Definition of SedConstructorException, the exception class for constructor exceptions
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
 * @class SedConstructorException
 * @ingroup Core
 * @brief Class of exceptions thrown by constructors of some libSed objects.
 *
 * In some situations, constructors for Sed objects may need to indicate to
 * callers that the creation of the object failed.  The failure may be for
 * different reasons, such as an attempt to use invalid parameters or a
 * system condition such as a memory error.  To communicate this to callers,
 * those classes will throw an SedConstructorException.
 *
 * In languages that don't have an exception mechanism (e.g., C), the
 * constructors generally try to return an error code instead of throwing
 * an exception.
 */

#ifndef SEDML_CONSTRUCTOR_EXCEPTION_H
#define SEDML_CONSTRUCTOR_EXCEPTION_H

#include <sedml/common/extern.h>
#include <sedml/SedNamespaces.h>

#ifdef __cplusplus

#include <string>
#include <stdexcept>
#include <algorithm>

LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedConstructorException : public std::invalid_argument
{
public:

  /** @cond doxygen-libsbml-internal */

  /* constructor */
  SedConstructorException(std::string errmsg = "");
  SedConstructorException(std::string errmsg, std::string sbmlErrMsg);
  SedConstructorException(std::string elementName, SedNamespaces* xmlns);
  virtual ~SedConstructorException() throw();

  /** @endcond */

  /**
   * Returns the message associated with this Sed exception.
   *
   * @return the message string.
   */
  const std::string getSedErrMsg() const { return mSedErrMsg; }

private:
  std::string mSedErrMsg;
};


LIBSEDML_CPP_NAMESPACE_END

#endif /* __cplusplus */

#endif /*SEDML_CONSTRUCTOR_EXCEPTION_H */

