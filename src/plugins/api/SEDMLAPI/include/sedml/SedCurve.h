/**
 * @file:   SedCurve.h
 * @brief:  Implementation of the SedCurve class
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


#ifndef SedCurve_H__
#define SedCurve_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedCurve : public SedBase
{

protected:

  std::string   mId;
  std::string   mName;
  bool          mLogX;
  bool          mIsSetLogX;
  bool          mLogY;
  bool          mIsSetLogY;
  std::string   mXDataReference;
  std::string   mYDataReference;


public:

  /**
   * Creates a new SedCurve with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedCurve
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedCurve
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedCurve
   */
  SedCurve(unsigned int level      = SEDML_DEFAULT_LEVEL,
           unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedCurve with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedCurve(SedNamespaces* sedns);


  /**
   * Copy constructor for SedCurve.
   *
   * @param orig; the SedCurve instance to copy.
   */
  SedCurve(const SedCurve& orig);


  /**
   * Assignment operator for SedCurve.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedCurve& operator=(const SedCurve& rhs);


  /**
   * Creates and returns a deep copy of this SedCurve object.
   *
   * @return a (deep) copy of this SedCurve object.
   */
  virtual SedCurve* clone() const;


  /**
   * Destructor for SedCurve.
   */
  virtual ~SedCurve();


  /**
   * Returns the value of the "id" attribute of this SedCurve.
   *
   * @return the value of the "id" attribute of this SedCurve as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "id" attribute has been set.
   *
   * @return @c true if this SedCurve's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedCurve.
   *
   * @param id; const std::string& value of the "id" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setId(const std::string& id);


  /**
   * Unsets the value of the "id" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetId();


  /**
   * Returns the value of the "name" attribute of this SedCurve.
   *
   * @return the value of the "name" attribute of this SedCurve as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "name" attribute has been set.
   *
   * @return @c true if this SedCurve's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedCurve.
   *
   * @param name; const std::string& value of the "name" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setName(const std::string& name);


  /**
   * Unsets the value of the "name" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetName();


  /**
   * Returns the value of the "logX" attribute of this SedCurve.
   *
   * @return the value of the "logX" attribute of this SedCurve as a boolean.
   */
  virtual const bool getLogX() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "logX" attribute has been set.
   *
   * @return @c true if this SedCurve's "logX" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLogX() const;


  /**
   * Sets the value of the "logX" attribute of this SedCurve.
   *
   * @param logX; bool value of the "logX" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLogX(bool logX);


  /**
   * Unsets the value of the "logX" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLogX();


  /**
   * Returns the value of the "logY" attribute of this SedCurve.
   *
   * @return the value of the "logY" attribute of this SedCurve as a boolean.
   */
  virtual const bool getLogY() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "logY" attribute has been set.
   *
   * @return @c true if this SedCurve's "logY" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLogY() const;


  /**
   * Sets the value of the "logY" attribute of this SedCurve.
   *
   * @param logY; bool value of the "logY" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLogY(bool logY);


  /**
   * Unsets the value of the "logY" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLogY();


  /**
   * Returns the value of the "xDataReference" attribute of this SedCurve.
   *
   * @return the value of the "xDataReference" attribute of this SedCurve as a string.
   */
  virtual const std::string& getXDataReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "xDataReference" attribute has been set.
   *
   * @return @c true if this SedCurve's "xDataReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetXDataReference() const;


  /**
   * Sets the value of the "xDataReference" attribute of this SedCurve.
   *
   * @param xDataReference; const std::string& value of the "xDataReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setXDataReference(const std::string& xDataReference);


  /**
   * Unsets the value of the "xDataReference" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetXDataReference();


  /**
   * Returns the value of the "yDataReference" attribute of this SedCurve.
   *
   * @return the value of the "yDataReference" attribute of this SedCurve as a string.
   */
  virtual const std::string& getYDataReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedCurve's "yDataReference" attribute has been set.
   *
   * @return @c true if this SedCurve's "yDataReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetYDataReference() const;


  /**
   * Sets the value of the "yDataReference" attribute of this SedCurve.
   *
   * @param yDataReference; const std::string& value of the "yDataReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setYDataReference(const std::string& yDataReference);


  /**
   * Unsets the value of the "yDataReference" attribute of this SedCurve.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetYDataReference();


  /**
   * Returns the XML element name of this object, which for SedCurve, is
   * always @c "sedCurve".
   *
   * @return the name of this element, i.e. @c "sedCurve".
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
   * for this SedCurve object have been set.
   *
   * @note The required attributes for a SedCurve object are:
   * @li "logX"
   * @li "logY"
   * @li "xDataReference"
   * @li "yDataReference"
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

class LIBSEDML_EXTERN SedListOfCurves : public SedListOf
{

public:

  /**
   * Creates a new SedListOfCurves with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfCurves
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfCurves
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfCurves
   */
  SedListOfCurves(unsigned int level      = SEDML_DEFAULT_LEVEL,
                  unsigned int version    = SEDML_DEFAULT_VERSION
                 );


  /**
   * Creates a new SedListOfCurves with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfCurves(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfCurves object.
   *
   * @return a (deep) copy of this SedListOfCurves object.
   */
  virtual SedListOfCurves* clone() const;


  /**
   * Get a Curve from the SedListOfCurves.
   *
   * @param n the index number of the Curve to get.
   *
   * @return the nth Curve in this SedListOfCurves.
   *
   * @see size()
   */
  virtual SedCurve* get(unsigned int n);


  /**
   * Get a Curve from the SedListOfCurves.
   *
   * @param n the index number of the Curve to get.
   *
   * @return the nth Curve in this SedListOfCurves.
   *
   * @see size()
   */
  virtual const SedCurve* get(unsigned int n) const;


  /**
   * Get a Curve from the SedListOfCurves
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Curve to get.
   *
   * @return Curve in this SedListOfCurves
   * with the given id or NULL if no such
   * Curve exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedCurve* get(const std::string& sid);


  /**
   * Get a Curve from the SedListOfCurves
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Curve to get.
   *
   * @return Curve in this SedListOfCurves
   * with the given id or NULL if no such
   * Curve exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedCurve* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Curve" to this SedListOfCurves.
   *
   * @param c; the Curve object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addCurve(const SedCurve* c);


  /**
   * Get the number of Curve objects in this SedListOfCurves.
   *
   * @return the number of Curve objects in this SedListOfCurves
   */
  unsigned int getNumCurves() const;


  /**
   * Creates a new Curve object, adds it to the
   * SedListOfCurves and returns the Curve object created.
   *
   * @return a new Curve object instance
   *
   * @see addCurve(const SedCurve* c)
   */
  SedCurve* createCurve();


  /**
   * Removes the nth Curve from this SedListOfCurves
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Curve to remove.
   *
   * @see size()
   */
  virtual SedCurve* remove(unsigned int n);


  /**
   * Removes the Curve from this SedListOfCurves with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Curve to remove.
   *
   * @return the Curve removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedCurve* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfCurves, is
   * always @c "sedListOfCurves".
   *
   * @return the name of this element, i.e. @c "sedListOfCurves".
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
   * Returns the libSEDML type code for the SEDML objects
   * contained in this SedListOf object
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
   * @return the SEDML type code for the objects in this SedListOf instance, or
   * @link SEDMLTypeCode_t#SEDML_UNKNOWN SEDML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getItemTypeCode() const;


protected:

  /** @cond doxygen-libsedml-internal */

  /**
   * Creates a new Curve in this SedListOfCurves
   */
  virtual SedBase* createObject(XMLInputStream& stream);


  /** @endcond doxygen-libsedml-internal */


  /** @cond doxygen-libsedml-internal */

  /**
   * Write the namespace for the Sed package.
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;


  /** @endcond doxygen-libsedml-internal */



};



LIBSEDML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSEDML_EXTERN
SedCurve_t *
SedCurve_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedCurve_free(SedCurve_t * sc);


LIBSEDML_EXTERN
SedCurve_t *
SedCurve_clone(SedCurve_t * sc);


LIBSEDML_EXTERN
char *
SedCurve_getId(SedCurve_t * sc);


LIBSEDML_EXTERN
char *
SedCurve_getName(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_getLogX(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_getLogY(SedCurve_t * sc);


LIBSEDML_EXTERN
char *
SedCurve_getXDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
char *
SedCurve_getYDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetId(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetName(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetLogX(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetLogY(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetXDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_isSetYDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_setId(SedCurve_t * sc, const char * id);


LIBSEDML_EXTERN
int
SedCurve_setName(SedCurve_t * sc, const char * name);


LIBSEDML_EXTERN
int
SedCurve_setLogX(SedCurve_t * sc, int logX);


LIBSEDML_EXTERN
int
SedCurve_setLogY(SedCurve_t * sc, int logY);


LIBSEDML_EXTERN
int
SedCurve_setXDataReference(SedCurve_t * sc, const char * xDataReference);


LIBSEDML_EXTERN
int
SedCurve_setYDataReference(SedCurve_t * sc, const char * yDataReference);


LIBSEDML_EXTERN
int
SedCurve_unsetId(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_unsetName(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_unsetLogX(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_unsetLogY(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_unsetXDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_unsetYDataReference(SedCurve_t * sc);


LIBSEDML_EXTERN
int
SedCurve_hasRequiredAttributes(SedCurve_t * sc);


LIBSEDML_EXTERN
SedCurve_t *
SedListOfCurves_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedCurve_t *
SedListOfCurves_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedCurve_H__  */

