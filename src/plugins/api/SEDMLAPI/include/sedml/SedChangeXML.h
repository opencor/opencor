/**
 * @file:   SedChangeXML.h
 * @brief:  Implementation of the SedChangeXML class
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


#ifndef SedChangeXML_H__
#define SedChangeXML_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedChange.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedChangeXML : public SedChange
{

protected:

  XMLNode*   mNewXML;


public:

  /**
   * Creates a new SedChangeXML with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedChangeXML
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedChangeXML
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedChangeXML
   */
  SedChangeXML(unsigned int level      = SEDML_DEFAULT_LEVEL,
               unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedChangeXML with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedChangeXML(SedNamespaces* sedns);


  /**
   * Copy constructor for SedChangeXML.
   *
   * @param orig; the SedChangeXML instance to copy.
   */
  SedChangeXML(const SedChangeXML& orig);


  /**
   * Assignment operator for SedChangeXML.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedChangeXML& operator=(const SedChangeXML& rhs);


  /**
   * Creates and returns a deep copy of this SedChangeXML object.
   *
   * @return a (deep) copy of this SedChangeXML object.
   */
  virtual SedChangeXML* clone() const;


  /**
   * Destructor for SedChangeXML.
   */
  virtual ~SedChangeXML();


  /**
   * Returns the value of the "newXML" attribute of this SedChangeXML.
   *
   * @return the value of the "newXML" attribute of this SedChangeXML as a XMLNode*.
   */
  virtual const XMLNode* getNewXML() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedChangeXML's "newXML" attribute has been set.
   *
   * @return @c true if this SedChangeXML's "newXML" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetNewXML() const;


  /**
   * Sets the value of the "newXML" attribute of this SedChangeXML.
   *
   * @param newXML; XMLNode* value of the "newXML" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setNewXML(XMLNode* newXML);


  /**
   * Unsets the value of the "newXML" attribute of this SedChangeXML.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetNewXML();


  /**
   * Returns the XML element name of this object, which for SedChangeXML, is
   * always @c "sedChangeXML".
   *
   * @return the name of this element, i.e. @c "sedChangeXML".
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
   * for this SedChangeXML object have been set.
   *
   * @note The required attributes for a SedChangeXML object are:
   * @li "newXML"
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
   * Subclasses should override this method ro read other XML.
   *
   * return true if read from stream, false otherwise.
   */
  virtual bool readOtherXML(XMLInputStream& stream);


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
SedChangeXML_t *
SedChangeXML_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedChangeXML_free(SedChangeXML_t * scxml);


LIBSEDML_EXTERN
SedChangeXML_t *
SedChangeXML_clone(SedChangeXML_t * scxml);


LIBSEDML_EXTERN
XMLNode_t*
SedChangeXML_getNewXML(SedChangeXML_t * scxml);


LIBSEDML_EXTERN
int
SedChangeXML_isSetNewXML(SedChangeXML_t * scxml);


LIBSEDML_EXTERN
int
SedChangeXML_setNewXML(SedChangeXML_t * scxml, XMLNode_t* newXML);


LIBSEDML_EXTERN
int
SedChangeXML_unsetNewXML(SedChangeXML_t * scxml);


LIBSEDML_EXTERN
int
SedChangeXML_hasRequiredAttributes(SedChangeXML_t * scxml);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedChangeXML_H__  */

