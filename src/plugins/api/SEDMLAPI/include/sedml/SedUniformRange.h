/**
 * @file:   SedUniformRange.h
 * @brief:  Implementation of the SedUniformRange class
 * @author: Frank T. Bergmann
 *
 * <!--------------------------------------------------------------------------
 * This file is part of libSEDML.  Please visit http://sed-ml.org for more
 * information about SED-ML. The latest version of libSEDML can be found on
 * github: https://github.com/fbergmann/libSEDML/
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
 * ------------------------------------------------------------------------ -->
 */


#ifndef SedUniformRange_H__
#define SedUniformRange_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedRange.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedUniformRange : public SedRange
{

protected:

  double        mStart;
  bool          mIsSetStart;
  double        mEnd;
  bool          mIsSetEnd;
  int           mNumberOfPoints;
  bool          mIsSetNumberOfPoints;
  std::string   mType;


public:

  /**
   * Creates a new SedUniformRange with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedUniformRange
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedUniformRange
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedUniformRange
   */
  SedUniformRange(unsigned int level      = SEDML_DEFAULT_LEVEL,
                  unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedUniformRange with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedUniformRange(SedNamespaces* sedns);


  /**
   * Copy constructor for SedUniformRange.
   *
   * @param orig; the SedUniformRange instance to copy.
   */
  SedUniformRange(const SedUniformRange& orig);


  /**
   * Assignment operator for SedUniformRange.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedUniformRange& operator=(const SedUniformRange& rhs);


  /**
   * Creates and returns a deep copy of this SedUniformRange object.
   *
   * @return a (deep) copy of this SedUniformRange object.
   */
  virtual SedUniformRange* clone() const;


  /**
   * Destructor for SedUniformRange.
   */
  virtual ~SedUniformRange();


  /**
   * Returns the value of the "start" attribute of this SedUniformRange.
   *
   * @return the value of the "start" attribute of this SedUniformRange as a double.
   */
  virtual const double getStart() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformRange's "start" attribute has been set.
   *
   * @return @c true if this SedUniformRange's "start" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetStart() const;


  /**
   * Sets the value of the "start" attribute of this SedUniformRange.
   *
   * @param start; double value of the "start" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setStart(double start);


  /**
   * Unsets the value of the "start" attribute of this SedUniformRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetStart();


  /**
   * Returns the value of the "end" attribute of this SedUniformRange.
   *
   * @return the value of the "end" attribute of this SedUniformRange as a double.
   */
  virtual const double getEnd() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformRange's "end" attribute has been set.
   *
   * @return @c true if this SedUniformRange's "end" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetEnd() const;


  /**
   * Sets the value of the "end" attribute of this SedUniformRange.
   *
   * @param end; double value of the "end" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setEnd(double end);


  /**
   * Unsets the value of the "end" attribute of this SedUniformRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetEnd();


  /**
   * Returns the value of the "numberOfPoints" attribute of this SedUniformRange.
   *
   * @return the value of the "numberOfPoints" attribute of this SedUniformRange as a integer.
   */
  virtual const int getNumberOfPoints() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformRange's "numberOfPoints" attribute has been set.
   *
   * @return @c true if this SedUniformRange's "numberOfPoints" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetNumberOfPoints() const;


  /**
   * Sets the value of the "numberOfPoints" attribute of this SedUniformRange.
   *
   * @param numberOfPoints; int value of the "numberOfPoints" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setNumberOfPoints(int numberOfPoints);


  /**
   * Unsets the value of the "numberOfPoints" attribute of this SedUniformRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetNumberOfPoints();


  /**
   * Returns the value of the "type" attribute of this SedUniformRange.
   *
   * @return the value of the "type" attribute of this SedUniformRange as a string.
   */
  virtual const std::string& getType() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformRange's "type" attribute has been set.
   *
   * @return @c true if this SedUniformRange's "type" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetType() const;


  /**
   * Sets the value of the "type" attribute of this SedUniformRange.
   *
   * @param type; const std::string& value of the "type" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setType(const std::string& type);


  /**
   * Unsets the value of the "type" attribute of this SedUniformRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetType();


  /**
   * Returns the XML element name of this object, which for SedUniformRange, is
   * always @c "sedUniformRange".
   *
   * @return the name of this element, i.e. @c "sedUniformRange".
   */
  virtual const std::string& getElementName() const;


  /**
   * Returns the libSEDML type code for this SEDML object.
   *
   * @if clike LibSEDML attaches an identifying code to every kind of SEDML
   * object.  These are known as <em>SEDML type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SEDMLTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SEDML_. @endif@if java LibSEDML attaches an identifying code to every
   * kind of SEDML object.  These are known as <em>SEDML type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSEDML, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsedmlConstants}.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if python LibSEDML attaches an identifying
   * code to every kind of SEDML object.  These are known as <em>SEDML type
   * codes</em>.  In the Python language interface for libSEDML, the type
   * codes are defined as static integer constants in the interface class
   * @link libsedml@endlink.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if csharp LibSEDML attaches an identifying
   * code to every kind of SEDML object.  These are known as <em>SEDML type
   * codes</em>.  In the C# language interface for libSEDML, the type codes
   * are defined as static integer constants in the interface class @link
   * libsedmlcs.libsedml@endlink.  The names of the type codes all begin with
   * the characters @c SEDML_. @endif
   *
   * @return the SEDML type code for this object, or
   * @link SEDMLTypeCode_t#SEDML_UNKNOWN SEDML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode() const;


  /**
   * Predicate returning @c true if all the required attributes
   * for this SedUniformRange object have been set.
   *
   * @note The required attributes for a SedUniformRange object are:
   * @li "start"
   * @li "end"
   * @li "numberOfPoints"
   * @li "type"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /** @cond doxygen-libsedml-internal */

  /**
   * Subclasses should override this method to write out their contained
   * SEDML objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
  virtual void writeElements(XMLOutputStream& stream) const;


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Accepts the given SedVisitor.
   */
  virtual bool accept(SedVisitor& v) const;


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Sets the parent SedDocument.
   */
  virtual void setSedDocument(SedDocument* d);


  /** @endcond doxygen-libsedml-internal */


protected:

  /** @cond doxygen-libsedml-internal */

  /**
   * return the SEDML object corresponding to next XMLToken.
   */
  virtual SedBase* createObject(XMLInputStream& stream);


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Get the list of expected attributes for this element.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Read values from the given XMLAttributes set into their specific fields.
   */
  virtual void readAttributes(const XMLAttributes& attributes,
                              const ExpectedAttributes& expectedAttributes);


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Write values of XMLAttributes to the output stream.
   */
  virtual void writeAttributes(XMLOutputStream& stream) const;


  /** @endcond doxygen-libsedml-internal */



};



LIBSEDML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSEDML_EXTERN
SedUniformRange_t *
SedUniformRange_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedUniformRange_free(SedUniformRange_t * sur);


LIBSEDML_EXTERN
SedUniformRange_t *
SedUniformRange_clone(SedUniformRange_t * sur);


LIBSEDML_EXTERN
double
SedUniformRange_getStart(SedUniformRange_t * sur);


LIBSEDML_EXTERN
double
SedUniformRange_getEnd(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_getNumberOfPoints(SedUniformRange_t * sur);


LIBSEDML_EXTERN
char *
SedUniformRange_getType(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_isSetStart(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_isSetEnd(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_isSetNumberOfPoints(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_isSetType(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_setStart(SedUniformRange_t * sur, double start);


LIBSEDML_EXTERN
int
SedUniformRange_setEnd(SedUniformRange_t * sur, double end);


LIBSEDML_EXTERN
int
SedUniformRange_setNumberOfPoints(SedUniformRange_t * sur, int numberOfPoints);


LIBSEDML_EXTERN
int
SedUniformRange_setType(SedUniformRange_t * sur, const char * type);


LIBSEDML_EXTERN
int
SedUniformRange_unsetStart(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_unsetEnd(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_unsetNumberOfPoints(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_unsetType(SedUniformRange_t * sur);


LIBSEDML_EXTERN
int
SedUniformRange_hasRequiredAttributes(SedUniformRange_t * sur);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedUniformRange_H__  */

