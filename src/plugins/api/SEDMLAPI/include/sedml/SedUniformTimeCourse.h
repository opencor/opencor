/**
 * @file:   SedUniformTimeCourse.h
 * @brief:  Implementation of the SedUniformTimeCourse class
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


#ifndef SedUniformTimeCourse_H__
#define SedUniformTimeCourse_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedSimulation.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedUniformTimeCourse : public SedSimulation
{

protected:

  double        mInitialTime;
  bool          mIsSetInitialTime;
  double        mOutputStartTime;
  bool          mIsSetOutputStartTime;
  double        mOutputEndTime;
  bool          mIsSetOutputEndTime;
  int           mNumberOfPoints;
  bool          mIsSetNumberOfPoints;


public:

  /**
   * Creates a new SedUniformTimeCourse with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedUniformTimeCourse
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedUniformTimeCourse
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedUniformTimeCourse
   */
  SedUniformTimeCourse(unsigned int level      = SEDML_DEFAULT_LEVEL,
                       unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedUniformTimeCourse with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedUniformTimeCourse(SedNamespaces* sedns);


  /**
   * Copy constructor for SedUniformTimeCourse.
   *
   * @param orig; the SedUniformTimeCourse instance to copy.
   */
  SedUniformTimeCourse(const SedUniformTimeCourse& orig);


  /**
   * Assignment operator for SedUniformTimeCourse.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedUniformTimeCourse& operator=(const SedUniformTimeCourse& rhs);


  /**
   * Creates and returns a deep copy of this SedUniformTimeCourse object.
   *
   * @return a (deep) copy of this SedUniformTimeCourse object.
   */
  virtual SedUniformTimeCourse* clone() const;


  /**
   * Destructor for SedUniformTimeCourse.
   */
  virtual ~SedUniformTimeCourse();


  /**
   * Returns the value of the "initialTime" attribute of this SedUniformTimeCourse.
   *
   * @return the value of the "initialTime" attribute of this SedUniformTimeCourse as a double.
   */
  virtual const double getInitialTime() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformTimeCourse's "initialTime" attribute has been set.
   *
   * @return @c true if this SedUniformTimeCourse's "initialTime" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetInitialTime() const;


  /**
   * Sets the value of the "initialTime" attribute of this SedUniformTimeCourse.
   *
   * @param initialTime; double value of the "initialTime" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setInitialTime(double initialTime);


  /**
   * Unsets the value of the "initialTime" attribute of this SedUniformTimeCourse.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetInitialTime();


  /**
   * Returns the value of the "outputStartTime" attribute of this SedUniformTimeCourse.
   *
   * @return the value of the "outputStartTime" attribute of this SedUniformTimeCourse as a double.
   */
  virtual const double getOutputStartTime() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformTimeCourse's "outputStartTime" attribute has been set.
   *
   * @return @c true if this SedUniformTimeCourse's "outputStartTime" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetOutputStartTime() const;


  /**
   * Sets the value of the "outputStartTime" attribute of this SedUniformTimeCourse.
   *
   * @param outputStartTime; double value of the "outputStartTime" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setOutputStartTime(double outputStartTime);


  /**
   * Unsets the value of the "outputStartTime" attribute of this SedUniformTimeCourse.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetOutputStartTime();


  /**
   * Returns the value of the "outputEndTime" attribute of this SedUniformTimeCourse.
   *
   * @return the value of the "outputEndTime" attribute of this SedUniformTimeCourse as a double.
   */
  virtual const double getOutputEndTime() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformTimeCourse's "outputEndTime" attribute has been set.
   *
   * @return @c true if this SedUniformTimeCourse's "outputEndTime" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetOutputEndTime() const;


  /**
   * Sets the value of the "outputEndTime" attribute of this SedUniformTimeCourse.
   *
   * @param outputEndTime; double value of the "outputEndTime" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setOutputEndTime(double outputEndTime);


  /**
   * Unsets the value of the "outputEndTime" attribute of this SedUniformTimeCourse.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetOutputEndTime();


  /**
   * Returns the value of the "numberOfPoints" attribute of this SedUniformTimeCourse.
   *
   * @return the value of the "numberOfPoints" attribute of this SedUniformTimeCourse as a integer.
   */
  virtual const int getNumberOfPoints() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedUniformTimeCourse's "numberOfPoints" attribute has been set.
   *
   * @return @c true if this SedUniformTimeCourse's "numberOfPoints" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetNumberOfPoints() const;


  /**
   * Sets the value of the "numberOfPoints" attribute of this SedUniformTimeCourse.
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
   * Unsets the value of the "numberOfPoints" attribute of this SedUniformTimeCourse.
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
   * Returns the XML element name of this object, which for SedUniformTimeCourse, is
   * always @c "sedUniformTimeCourse".
   *
   * @return the name of this element, i.e. @c "sedUniformTimeCourse".
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
   * for this SedUniformTimeCourse object have been set.
   *
   * @note The required attributes for a SedUniformTimeCourse object are:
   * @li "initialTime"
   * @li "outputStartTime"
   * @li "outputEndTime"
   * @li "numberOfPoints"
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
SedUniformTimeCourse_t *
SedUniformTimeCourse_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedUniformTimeCourse_free(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
SedUniformTimeCourse_t *
SedUniformTimeCourse_clone(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
double
SedUniformTimeCourse_getInitialTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
double
SedUniformTimeCourse_getOutputStartTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
double
SedUniformTimeCourse_getOutputEndTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_getNumberOfPoints(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_isSetInitialTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_isSetOutputStartTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_isSetOutputEndTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_isSetNumberOfPoints(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_setInitialTime(SedUniformTimeCourse_t * sutc, double initialTime);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_setOutputStartTime(SedUniformTimeCourse_t * sutc, double outputStartTime);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_setOutputEndTime(SedUniformTimeCourse_t * sutc, double outputEndTime);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_setNumberOfPoints(SedUniformTimeCourse_t * sutc, int numberOfPoints);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_unsetInitialTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_unsetOutputStartTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_unsetOutputEndTime(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_unsetNumberOfPoints(SedUniformTimeCourse_t * sutc);


LIBSEDML_EXTERN
int
SedUniformTimeCourse_hasRequiredAttributes(SedUniformTimeCourse_t * sutc);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedUniformTimeCourse_H__  */

