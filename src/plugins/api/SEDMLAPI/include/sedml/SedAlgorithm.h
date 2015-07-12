/**
 * @file:   SedAlgorithm.h
 * @brief:  Implementation of the SedAlgorithm class
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


#ifndef SedAlgorithm_H__
#define SedAlgorithm_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedAlgorithmParameter.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedAlgorithm : public SedBase
{

protected:

  SedListOfAlgorithmParameters   mAlgorithmParameters;
  std::string   mKisaoID;


public:

  /**
   * Creates a new SedAlgorithm with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedAlgorithm
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedAlgorithm
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedAlgorithm
   */
  SedAlgorithm(unsigned int level      = SEDML_DEFAULT_LEVEL,
               unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedAlgorithm with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedAlgorithm(SedNamespaces* sedns);


  /**
   * Copy constructor for SedAlgorithm.
   *
   * @param orig; the SedAlgorithm instance to copy.
   */
  SedAlgorithm(const SedAlgorithm& orig);


  /**
   * Assignment operator for SedAlgorithm.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedAlgorithm& operator=(const SedAlgorithm& rhs);


  /**
   * Creates and returns a deep copy of this SedAlgorithm object.
   *
   * @return a (deep) copy of this SedAlgorithm object.
   */
  virtual SedAlgorithm* clone() const;


  /**
   * Destructor for SedAlgorithm.
   */
  virtual ~SedAlgorithm();


  /**
   * Returns the value of the "kisaoID" attribute of this SedAlgorithm.
   *
   * @return the value of the "kisaoID" attribute of this SedAlgorithm as a string.
   */
  virtual const std::string& getKisaoID() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedAlgorithm's "kisaoID" attribute has been set.
   *
   * @return @c true if this SedAlgorithm's "kisaoID" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetKisaoID() const;


  /**
   * Sets the value of the "kisaoID" attribute of this SedAlgorithm.
   *
   * @param kisaoID; const std::string& value of the "kisaoID" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setKisaoID(const std::string& kisaoID);


  /**
   * Unsets the value of the "kisaoID" attribute of this SedAlgorithm.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetKisaoID();


  /**
   * Returns the  "SedListOfAlgorithmParameters" in this SedAlgorithm object.
   *
   * @return the "SedListOfAlgorithmParameters" attribute of this SedAlgorithm.
   */
  const SedListOfAlgorithmParameters* getListOfAlgorithmParameters() const;


  /**
   * Get a AlgorithmParameter from the SedListOfAlgorithmParameters.
   *
   * @param n the index number of the AlgorithmParameter to get.
   *
   * @return the nth AlgorithmParameter in the SedListOfAlgorithmParameters within this SedAlgorithm.
   *
   * @see getNumAlgorithmParameters()
   */
  SedAlgorithmParameter* getAlgorithmParameter(unsigned int n);


  /**
   * Get a AlgorithmParameter from the SedListOfAlgorithmParameters.
   *
   * @param n the index number of the AlgorithmParameter to get.
   *
   * @return the nth AlgorithmParameter in the SedListOfAlgorithmParameters within this SedAlgorithm.
   *
   * @see getNumAlgorithmParameters()
   */
  const SedAlgorithmParameter* getAlgorithmParameter(unsigned int n) const;


  /**
   * Get a AlgorithmParameter from the SedListOfAlgorithmParameters
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the AlgorithmParameter to get.
   *
   * @return the AlgorithmParameter in the SedListOfAlgorithmParameters
   * with the given id or NULL if no such
   * AlgorithmParameter exists.
   *
   * @see getAlgorithmParameter(unsigned int n)
   *
   * @see getNumAlgorithmParameters()
   */
  SedAlgorithmParameter* getAlgorithmParameter(const std::string& sid);


  /**
   * Get a AlgorithmParameter from the SedListOfAlgorithmParameters
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the AlgorithmParameter to get.
   *
   * @return the AlgorithmParameter in the SedListOfAlgorithmParameters
   * with the given id or NULL if no such
   * AlgorithmParameter exists.
   *
   * @see getAlgorithmParameter(unsigned int n)
   *
   * @see getNumAlgorithmParameters()
   */
  const SedAlgorithmParameter* getAlgorithmParameter(const std::string& sid) const;


  /**
   * Adds a copy the given "SedAlgorithmParameter" to this SedAlgorithm.
   *
   * @param sap; the SedAlgorithmParameter object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addAlgorithmParameter(const SedAlgorithmParameter* sap);


  /**
   * Get the number of SedAlgorithmParameter objects in this SedAlgorithm.
   *
   * @return the number of SedAlgorithmParameter objects in this SedAlgorithm
   */
  unsigned int getNumAlgorithmParameters() const;


  /**
   * Creates a new SedAlgorithmParameter object, adds it to this SedAlgorithms
   * SedListOfAlgorithmParameters and returns the SedAlgorithmParameter object created.
   *
   * @return a new SedAlgorithmParameter object instance
   *
   * @see addAlgorithmParameter(const SedAlgorithmParameter* sap)
   */
  SedAlgorithmParameter* createAlgorithmParameter();


  /**
   * Removes the nth AlgorithmParameter from the SedListOfAlgorithmParameters within this SedAlgorithm.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the AlgorithmParameter to remove.
   *
   * @see getNumAlgorithmParameters()
   */
  SedAlgorithmParameter* removeAlgorithmParameter(unsigned int n);


  /**
   * Removes the AlgorithmParameter with the given identifier from the SedListOfAlgorithmParameters within this SedAlgorithm
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the AlgorithmParameter to remove.
   *
   * @return the AlgorithmParameter removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedAlgorithmParameter* removeAlgorithmParameter(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedAlgorithm, is
   * always @c "sedAlgorithm".
   *
   * @return the name of this element, i.e. @c "sedAlgorithm".
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
   * for this SedAlgorithm object have been set.
   *
   * @note The required attributes for a SedAlgorithm object are:
   * @li "kisaoID"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedAlgorithm object have been set.
   *
   * @note The required elements for a SedAlgorithm object are:
   *
   * @return a boolean value indicating whether all the required
   * elements for this object have been defined.
   */
  virtual bool hasRequiredElements() const;


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


  /** @cond doxygen-libsedml-internal */

  /**
   * Connects to child elements.
   */
  virtual void connectToChild();


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
SedAlgorithm_t *
SedAlgorithm_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedAlgorithm_free(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
SedAlgorithm_t *
SedAlgorithm_clone(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
char *
SedAlgorithm_getKisaoID(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
int
SedAlgorithm_isSetKisaoID(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
int
SedAlgorithm_setKisaoID(SedAlgorithm_t * sa, const char * kisaoID);


LIBSEDML_EXTERN
int
SedAlgorithm_unsetKisaoID(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
int
SedAlgorithm_addAlgorithmParameter(SedAlgorithm_t * sa, SedAlgorithmParameter_t * sap);


LIBSEDML_EXTERN
SedAlgorithmParameter_t *
SedAlgorithm_createAlgorithmParameter(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
SedListOf_t *
SedAlgorithm_getSedListOfAlgorithmParameters(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
SedAlgorithmParameter_t *
SedAlgorithm_getAlgorithmParameter(SedAlgorithm_t * sa, unsigned int n);


LIBSEDML_EXTERN
SedAlgorithmParameter_t *
SedAlgorithm_getAlgorithmParameterById(SedAlgorithm_t * sa, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedAlgorithm_getNumAlgorithmParameters(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
SedAlgorithmParameter_t *
SedAlgorithm_removeAlgorithmParameter(SedAlgorithm_t * sa, unsigned int n);


LIBSEDML_EXTERN
SedAlgorithmParameter_t *
SedAlgorithm_removeAlgorithmParameterById(SedAlgorithm_t * sa, const char * sid);


LIBSEDML_EXTERN
int
SedAlgorithm_hasRequiredAttributes(SedAlgorithm_t * sa);


LIBSEDML_EXTERN
int
SedAlgorithm_hasRequiredElements(SedAlgorithm_t * sa);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedAlgorithm_H__  */

