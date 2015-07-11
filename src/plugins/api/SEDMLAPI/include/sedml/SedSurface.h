/**
 * @file:   SedSurface.h
 * @brief:  Implementation of the SedSurface class
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


#ifndef SedSurface_H__
#define SedSurface_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedCurve.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedSurface : public SedCurve
{

protected:

  bool          mLogZ;
  bool          mIsSetLogZ;
  std::string   mZDataReference;


public:

  /**
   * Creates a new SedSurface with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedSurface
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedSurface
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedSurface
   */
  SedSurface(unsigned int level      = SEDML_DEFAULT_LEVEL,
             unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedSurface with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedSurface(SedNamespaces* sedns);


  /**
   * Copy constructor for SedSurface.
   *
   * @param orig; the SedSurface instance to copy.
   */
  SedSurface(const SedSurface& orig);


  /**
   * Assignment operator for SedSurface.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedSurface& operator=(const SedSurface& rhs);


  /**
   * Creates and returns a deep copy of this SedSurface object.
   *
   * @return a (deep) copy of this SedSurface object.
   */
  virtual SedSurface* clone() const;


  /**
   * Destructor for SedSurface.
   */
  virtual ~SedSurface();


  /**
   * Returns the value of the "logZ" attribute of this SedSurface.
   *
   * @return the value of the "logZ" attribute of this SedSurface as a boolean.
   */
  virtual const bool getLogZ() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSurface's "logZ" attribute has been set.
   *
   * @return @c true if this SedSurface's "logZ" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLogZ() const;


  /**
   * Sets the value of the "logZ" attribute of this SedSurface.
   *
   * @param logZ; bool value of the "logZ" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLogZ(bool logZ);


  /**
   * Unsets the value of the "logZ" attribute of this SedSurface.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLogZ();


  /**
   * Returns the value of the "zDataReference" attribute of this SedSurface.
   *
   * @return the value of the "zDataReference" attribute of this SedSurface as a string.
   */
  virtual const std::string& getZDataReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSurface's "zDataReference" attribute has been set.
   *
   * @return @c true if this SedSurface's "zDataReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetZDataReference() const;


  /**
   * Sets the value of the "zDataReference" attribute of this SedSurface.
   *
   * @param zDataReference; const std::string& value of the "zDataReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setZDataReference(const std::string& zDataReference);


  /**
   * Unsets the value of the "zDataReference" attribute of this SedSurface.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetZDataReference();


  /**
   * Returns the XML element name of this object, which for SedSurface, is
   * always @c "sedSurface".
   *
   * @return the name of this element, i.e. @c "sedSurface".
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
   * for this SedSurface object have been set.
   *
   * @note The required attributes for a SedSurface object are:
   * @li "logZ"
   * @li "zDataReference"
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

class LIBSEDML_EXTERN SedListOfSurfaces : public SedListOf
{

public:

  /**
   * Creates a new SedListOfSurfaces with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfSurfaces
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfSurfaces
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfSurfaces
   */
  SedListOfSurfaces(unsigned int level      = SEDML_DEFAULT_LEVEL,
                    unsigned int version    = SEDML_DEFAULT_VERSION
                   );


  /**
   * Creates a new SedListOfSurfaces with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfSurfaces(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfSurfaces object.
   *
   * @return a (deep) copy of this SedListOfSurfaces object.
   */
  virtual SedListOfSurfaces* clone() const;


  /**
   * Get a Surface from the SedListOfSurfaces.
   *
   * @param n the index number of the Surface to get.
   *
   * @return the nth Surface in this SedListOfSurfaces.
   *
   * @see size()
   */
  virtual SedSurface* get(unsigned int n);


  /**
   * Get a Surface from the SedListOfSurfaces.
   *
   * @param n the index number of the Surface to get.
   *
   * @return the nth Surface in this SedListOfSurfaces.
   *
   * @see size()
   */
  virtual const SedSurface* get(unsigned int n) const;


  /**
   * Get a Surface from the SedListOfSurfaces
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Surface to get.
   *
   * @return Surface in this SedListOfSurfaces
   * with the given id or NULL if no such
   * Surface exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedSurface* get(const std::string& sid);


  /**
   * Get a Surface from the SedListOfSurfaces
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Surface to get.
   *
   * @return Surface in this SedListOfSurfaces
   * with the given id or NULL if no such
   * Surface exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedSurface* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Surface" to this SedListOfSurfaces.
   *
   * @param s; the Surface object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addSurface(const SedSurface* s);


  /**
   * Get the number of Surface objects in this SedListOfSurfaces.
   *
   * @return the number of Surface objects in this SedListOfSurfaces
   */
  unsigned int getNumSurfaces() const;


  /**
   * Creates a new Surface object, adds it to the
   * SedListOfSurfaces and returns the Surface object created.
   *
   * @return a new Surface object instance
   *
   * @see addSurface(const SedSurface* s)
   */
  SedSurface* createSurface();


  /**
   * Removes the nth Surface from this SedListOfSurfaces
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Surface to remove.
   *
   * @see size()
   */
  virtual SedSurface* remove(unsigned int n);


  /**
   * Removes the Surface from this SedListOfSurfaces with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Surface to remove.
   *
   * @return the Surface removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedSurface* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfSurfaces, is
   * always @c "sedListOfSurfaces".
   *
   * @return the name of this element, i.e. @c "sedListOfSurfaces".
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
   * Creates a new Surface in this SedListOfSurfaces
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
SedSurface_t *
SedSurface_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedSurface_free(SedSurface_t * ss);


LIBSEDML_EXTERN
SedSurface_t *
SedSurface_clone(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_getLogZ(SedSurface_t * ss);


LIBSEDML_EXTERN
char *
SedSurface_getZDataReference(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_isSetLogZ(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_isSetZDataReference(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_setLogZ(SedSurface_t * ss, int logZ);


LIBSEDML_EXTERN
int
SedSurface_setZDataReference(SedSurface_t * ss, const char * zDataReference);


LIBSEDML_EXTERN
int
SedSurface_unsetLogZ(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_unsetZDataReference(SedSurface_t * ss);


LIBSEDML_EXTERN
int
SedSurface_hasRequiredAttributes(SedSurface_t * ss);


LIBSEDML_EXTERN
SedSurface_t *
SedListOfSurfaces_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedSurface_t *
SedListOfSurfaces_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedSurface_H__  */

