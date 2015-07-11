/**
 * @file    SedErrorLog.h
 * @brief   Stores errors (and messages) encountered while processing Sed.
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
 * @class SedErrorLog
 * @ingroup Core
 * @brief Log of errors and other events encountered during Sed processing.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The error log is a list.  Each SedDocument maintains its own
 * SedErrorLog.  When a libSed operation on Sed content results in an
 * error, or when there is something worth noting about the Sed content,
 * the issue is reported as an SedError object stored in the SedErrorLog
 * list.
 *
 * SedErrorLog is derived from XMLErrorLog, an object class that serves
 * exactly the same purpose but for the XML parsing layer.  XMLErrorLog
 * provides crucial methods such as
 * @if java XMLErrorLog::getNumErrors()@else getNumErrors()@endif@~
 * for determining how many SedError or XMLError objects are in the log.
 * SedErrorLog inherits these methods.
 *
 * The general approach to working with SedErrorLog in user programs
 * involves first obtaining a pointer to a log from a libSed object such
 * as SedDocument.  Callers should then use
 * @if java XMLErrorLog::getNumErrors()@else getNumErrors() @endif@~ to inquire how
 * many objects there are in the list.  (The answer may be 0.)  If there is
 * at least one SedError object in the SedErrorLog instance, callers can
 * then iterate over the list using
 * SedErrorLog::getError(@if java long n@endif)@if clike const@endif,
 * using methods provided by the SedError class to find out the error code
 * and associated information such as the error severity, the message, and
 * the line number in the input.
 *
 * If you wish to simply print the error strings for a human to read, an
 * easier and more direct way might be to use SedDocument::printErrors().
 *
 * @see SedError
 * @see XMLErrorLog
 * @see XMLError
 */

#ifndef SedErrorLog_h
#define SedErrorLog_h


#include <sedml/common/extern.h>
#include <sbml/xml/XMLError.h>
#include <sbml/xml/XMLErrorLog.h>
#include <sedml/SedError.h>


#ifdef __cplusplus

#include <vector>

LIBSEDML_CPP_NAMESPACE_BEGIN

class LIBSEDML_EXTERN SedErrorLog : public XMLErrorLog
{
public:

  /**
   * Returns the <i>n</i>th SedError object in this log.
   *
   * Index @p n is counted from 0.  Callers should first inquire about the
   * number of items in the log by using the
   * @if java XMLErrorLog::getNumErrors()@else getNumErrors()@endif@~ method.
   * Attempts to use an error index number that exceeds the actual number
   * of errors in the log will result in a @c NULL being returned.
   *
   * @param n the index number of the error to retrieve (with 0 being the
   * first error).
   *
   * @return the <i>n</i>th SedError in this log, or @c NULL if @p n is
   * greater than or equal to
   * @if java XMLErrorLog::getNumErrors()@else getNumErrors()@endif.
   *
   * @see getNumErrors()
   */
  const SedError* getError(unsigned int n) const;


  /**
   * Returns the number of errors that have been logged with the given
   * severity code.
   *
   * LibSed associates severity levels with every SedError object to
   * provide an indication of how serious the problem is.  Severities range
   * from informational diagnostics to fatal (irrecoverable) errors.  Given
   * an SedError object instance, a caller can interrogate it for its
   * severity level using methods such as SedError::getSeverity(),
   * SedError::isFatal(), and so on.  The present method encapsulates
   * iteration and interrogation of all objects in an SedErrorLog, making
   * it easy to check for the presence of error objects with specific
   * severity levels.
   *
   * @if clike @param severity a value from
   * #SedErrorSeverity_t @endif@if java @param severity a
   * value from the set of <code>LIBSEDML_SEV_</code> constants defined by
   * the interface class <code><a
   * href="libsbmlConstants.html">libsbmlConstants</a></code> @endif@if python @param severity a
   * value from the set of <code>LIBSEDML_SEV_</code> constants defined by
   * the interface class @link libsbml libsbml@endlink. @endif@~
   *
   * @return a count of the number of errors with the given severity code.
   *
   * @see getNumErrors()
   */
  unsigned int getNumFailsWithSeverity(unsigned int severity);

  /**
  * Returns the number of errors that have been logged with the given
  * severity code.
  *
  * LibSed associates severity levels with every SedError object to
  * provide an indication of how serious the problem is.  Severities range
  * from informational diagnostics to fatal (irrecoverable) errors.  Given
  * an SedError object instance, a caller can interrogate it for its
  * severity level using methods such as SedError::getSeverity(),
  * SedError::isFatal(), and so on.  The present method encapsulates
  * iteration and interrogation of all objects in an SedErrorLog, making
  * it easy to check for the presence of error objects with specific
  * severity levels.
  *
  * @if clike @param severity a value from
  * #SedErrorSeverity_t @endif@if java @param severity a
  * value from the set of <code>LIBSEDML_SEV_</code> constants defined by
  * the interface class <code><a
  * href="libsbmlConstants.html">libsbmlConstants</a></code> @endif@if python @param severity a
  * value from the set of <code>LIBSEDML_SEV_</code> constants defined by
  * the interface class @link libsbml libsbml@endlink. @endif@~
  *
  * @return a count of the number of errors with the given severity code.
  *
  * @see getNumErrors()
  */
  unsigned int getNumFailsWithSeverity(unsigned int severity) const;


  /** @cond doxygen-libsbml-internal */

  /**
   * Creates a new, empty SedErrorLog.
   */
  SedErrorLog();


  /**
   * Destroys this SedErrorLog.
   */
  virtual ~SedErrorLog();


  /**
   * Convenience function that combines creating an SedError object and
   * adding it to the log.
   *
   * @param errorId an unsigned int, the identification number of the error.
   *
   * @param level an unsigned int, the Sed Level
   *
   * @param version an unsigned int, the Sed Level's Version
   *
   * @param details a string containing additional details about the error.
   * If the error code in @p errorId is one that is recognized by SedError,
   * the given message is @em appended to a predefined message associated
   * with the given code.  If the error code is not recognized, the message
   * is stored as-is as the text of the error.
   *
   * @param line an unsigned int, the line number at which the error occured.
   *
   * @param column an unsigned int, the column number at which the error occured.
   *
   * @param severity an integer indicating severity of the error.
   *
   * @param category an integer indicating the category to which the error
   * belongs.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void logError
  (
    const unsigned int errorId  = 0
                                  , const unsigned int level    = SEDML_DEFAULT_LEVEL
                                      , const unsigned int version  = SEDML_DEFAULT_VERSION
                                          , const std::string& details  = ""
                                              , const unsigned int line     = 0
                                                  , const unsigned int column   = 0
                                                      , const unsigned int severity = LIBSEDML_SEV_ERROR
                                                          , const unsigned int category = LIBSEDML_CAT_SEDML
  );


  void logPackageError
  (
    const std::string& package    = "core"
                                    , const unsigned int errorId    = 0
                                        , const unsigned int pkgVersion = 1
                                            , const unsigned int level      = SEDML_DEFAULT_LEVEL
                                                , const unsigned int version    = SEDML_DEFAULT_VERSION
                                                    , const std::string& details    = ""
                                                        , const unsigned int line       = 0
                                                            , const unsigned int column     = 0
                                                                , const unsigned int severity   = LIBSEDML_SEV_ERROR
                                                                    , const unsigned int category   = LIBSEDML_CAT_SEDML
  );


  /**
   * Adds the given SedError to the log.
   *
   * @param error SedError, the error to be logged.
   */
  void add(const SedError& error);


  /** @cond doxygen-cpp-only */

  /**
   * Adds (copies) the SedErrors in the given SedError list to this
   * SedErrorLog.
   *
   * @param errors list, a list of SedError to be added to the log.
   */
  void add(const std::list<SedError>& errors);


  /**
   * Adds (copies) the SedErrors in the given SedError vector to this
   * SedErrorLog.
   *
   * @param errors vector, a vector of SedError to be added to the log.
   */
  void add(const std::vector<SedError>& errors);

  /** @endcond */


  /**
   * Removes an error having errorId from the SedError list.
   *
   * Only the first item will be removed if there are multiple errors
   * with the given errorId.
   *
   * @param errorId the error identifier of the error to be removed.
   */
  void remove(const unsigned int errorId);


  /**
   * Returns true if SedErrorLog contains an errorId
   *
   * @param errorId the error identifier of the error to be found.
   */
  bool contains(const unsigned int errorId);


  /** @endcond */
};

LIBSEDML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* SedErrorLog_h */
