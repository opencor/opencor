/**
 * @file:   SedChangeAttribute.h
 * @brief:  Implementation of the SedChangeAttribute class
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


#ifndef SedChangeAttribute_H__
#define SedChangeAttribute_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedChange.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedChangeAttribute : public SedChange
{

protected:

  std::string   mNewValue;


public:

  /**
   * Creates a new SedChangeAttribute with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedChangeAttribute
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedChangeAttribute
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedChangeAttribute
   */
  SedChangeAttribute(unsigned int level      = SEDML_DEFAULT_LEVEL,
                     unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedChangeAttribute with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedChangeAttribute(SedNamespaces* sedns);


  /**
   * Copy constructor for SedChangeAttribute.
   *
   * @param orig; the SedChangeAttribute instance to copy.
   */
  SedChangeAttribute(const SedChangeAttribute& orig);


  /**
   * Assignment operator for SedChangeAttribute.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedChangeAttribute& operator=(const SedChangeAttribute& rhs);


  /**
   * Creates and returns a deep copy of this SedChangeAttribute object.
   *
   * @return a (deep) copy of this SedChangeAttribute object.
   */
  virtual SedChangeAttribute* clone() const;


  /**
   * Destructor for SedChangeAttribute.
   */
  virtual ~SedChangeAttribute();


  /**
   * Returns the value of the "newValue" attribute of this SedChangeAttribute.
   *
   * @return the value of the "newValue" attribute of this SedChangeAttribute as a string.
   */
  virtual const std::string& getNewValue() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedChangeAttribute's "newValue" attribute has been set.
   *
   * @return @c true if this SedChangeAttribute's "newValue" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetNewValue() const;


  /**
   * Sets the value of the "newValue" attribute of this SedChangeAttribute.
   *
   * @param newValue; const std::string& value of the "newValue" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setNewValue(const std::string& newValue);


  /**
   * Unsets the value of the "newValue" attribute of this SedChangeAttribute.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetNewValue();


  /**
   * Returns the XML element name of this object, which for SedChangeAttribute, is
   * always @c "sedChangeAttribute".
   *
   * @return the name of this element, i.e. @c "sedChangeAttribute".
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
   * for this SedChangeAttribute object have been set.
   *
   * @note The required attributes for a SedChangeAttribute object are:
   * @li "newValue"
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
SedChangeAttribute_t *
SedChangeAttribute_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedChangeAttribute_free(SedChangeAttribute_t * sca);


LIBSEDML_EXTERN
SedChangeAttribute_t *
SedChangeAttribute_clone(SedChangeAttribute_t * sca);


LIBSEDML_EXTERN
char *
SedChangeAttribute_getNewValue(SedChangeAttribute_t * sca);


LIBSEDML_EXTERN
int
SedChangeAttribute_isSetNewValue(SedChangeAttribute_t * sca);


LIBSEDML_EXTERN
int
SedChangeAttribute_setNewValue(SedChangeAttribute_t * sca, const char * newValue);


LIBSEDML_EXTERN
int
SedChangeAttribute_unsetNewValue(SedChangeAttribute_t * sca);


LIBSEDML_EXTERN
int
SedChangeAttribute_hasRequiredAttributes(SedChangeAttribute_t * sca);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedChangeAttribute_H__  */

