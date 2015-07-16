/**
 * @file:   SedRange.h
 * @brief:  Implementation of the SedRange class
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


#ifndef SedRange_H__
#define SedRange_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedRange : public SedBase
{

protected:

  std::string   mId;


public:

  /**
   * Creates a new SedRange with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedRange
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedRange
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedRange
   */
  SedRange(unsigned int level      = SEDML_DEFAULT_LEVEL,
           unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedRange with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedRange(SedNamespaces* sedns);


  /**
   * Copy constructor for SedRange.
   *
   * @param orig; the SedRange instance to copy.
   */
  SedRange(const SedRange& orig);


  /**
   * Assignment operator for SedRange.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedRange& operator=(const SedRange& rhs);


  /**
   * Creates and returns a deep copy of this SedRange object.
   *
   * @return a (deep) copy of this SedRange object.
   */
  virtual SedRange* clone() const;


  /**
   * Destructor for SedRange.
   */
  virtual ~SedRange();


  /**
   * Returns the value of the "id" attribute of this SedRange.
   *
   * @return the value of the "id" attribute of this SedRange as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedRange's "id" attribute has been set.
   *
   * @return @c true if this SedRange's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedRange.
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
   * Unsets the value of the "id" attribute of this SedRange.
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
   * Returns the XML element name of this object, which for SedRange, is
   * always @c "sedRange".
   *
   * @return the name of this element, i.e. @c "sedRange".
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
   * for this SedRange object have been set.
   *
   * @note The required attributes for a SedRange object are:
   * @li "id"
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

class LIBSEDML_EXTERN SedListOfRanges : public SedListOf
{

public:

  /**
   * Creates a new SedListOfRanges with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfRanges
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfRanges
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfRanges
   */
  SedListOfRanges(unsigned int level      = SEDML_DEFAULT_LEVEL,
                  unsigned int version    = SEDML_DEFAULT_VERSION
                 );


  /**
   * Creates a new SedListOfRanges with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfRanges(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfRanges object.
   *
   * @return a (deep) copy of this SedListOfRanges object.
   */
  virtual SedListOfRanges* clone() const;


  /**
   * Get a Range from the SedListOfRanges.
   *
   * @param n the index number of the Range to get.
   *
   * @return the nth Range in this SedListOfRanges.
   *
   * @see size()
   */
  virtual SedRange* get(unsigned int n);


  /**
   * Get a Range from the SedListOfRanges.
   *
   * @param n the index number of the Range to get.
   *
   * @return the nth Range in this SedListOfRanges.
   *
   * @see size()
   */
  virtual const SedRange* get(unsigned int n) const;


  /**
   * Get a Range from the SedListOfRanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Range to get.
   *
   * @return Range in this SedListOfRanges
   * with the given id or NULL if no such
   * Range exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedRange* get(const std::string& sid);


  /**
   * Get a Range from the SedListOfRanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Range to get.
   *
   * @return Range in this SedListOfRanges
   * with the given id or NULL if no such
   * Range exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedRange* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Range" to this SedListOfRanges.
   *
   * @param r; the Range object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addRange(const SedRange* r);


  /**
   * Get the number of Range objects in this SedListOfRanges.
   *
   * @return the number of Range objects in this SedListOfRanges
   */
  unsigned int getNumRanges() const;


  /**
   * Creates a new Range object, adds it to the
   * SedListOfRanges and returns the Range object created.
   *
   * @return a new Range object instance
   *
   * @see addRange(const SedRange* r)
   */
  SedUniformRange* createUniformRange();


  /**
   * Creates a new Range object, adds it to the
   * SedListOfRanges and returns the Range object created.
   *
   * @return a new Range object instance
   *
   * @see addRange(const SedRange* r)
   */
  SedVectorRange* createVectorRange();


  /**
   * Creates a new Range object, adds it to the
   * SedListOfRanges and returns the Range object created.
   *
   * @return a new Range object instance
   *
   * @see addRange(const SedRange* r)
   */
  SedFunctionalRange* createFunctionalRange();


  /**
   * Removes the nth Range from this SedListOfRanges
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Range to remove.
   *
   * @see size()
   */
  virtual SedRange* remove(unsigned int n);


  /**
   * Removes the Range from this SedListOfRanges with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Range to remove.
   *
   * @return the Range removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedRange* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfRanges, is
   * always @c "sedListOfRanges".
   *
   * @return the name of this element, i.e. @c "sedListOfRanges".
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
   * Creates a new Range in this SedListOfRanges
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
SedRange_t *
SedRange_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedRange_free(SedRange_t * sr);


LIBSEDML_EXTERN
SedRange_t *
SedRange_clone(SedRange_t * sr);


LIBSEDML_EXTERN
char *
SedRange_getId(SedRange_t * sr);


LIBSEDML_EXTERN
int
SedRange_isSetId(SedRange_t * sr);


LIBSEDML_EXTERN
int
SedRange_setId(SedRange_t * sr, const char * id);


LIBSEDML_EXTERN
int
SedRange_unsetId(SedRange_t * sr);


LIBSEDML_EXTERN
int
SedRange_hasRequiredAttributes(SedRange_t * sr);


LIBSEDML_EXTERN
SedRange_t *
SedListOfRanges_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedRange_t *
SedListOfRanges_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedRange_H__  */

