/**
 * @file:   SedFunctionalRange.h
 * @brief:  Implementation of the SedFunctionalRange class
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


#ifndef SedFunctionalRange_H__
#define SedFunctionalRange_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedRange.h>
#include <sedml/SedVariable.h>
#include <sedml/SedParameter.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedFunctionalRange : public SedRange
{

protected:

  SedListOfVariables   mVariables;
  SedListOfParameters   mParameters;
  std::string   mRange;
  ASTNode*      mMath;


public:

  /**
   * Creates a new SedFunctionalRange with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedFunctionalRange
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedFunctionalRange
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedFunctionalRange
   */
  SedFunctionalRange(unsigned int level      = SEDML_DEFAULT_LEVEL,
                     unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedFunctionalRange with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedFunctionalRange(SedNamespaces* sedns);


  /**
   * Copy constructor for SedFunctionalRange.
   *
   * @param orig; the SedFunctionalRange instance to copy.
   */
  SedFunctionalRange(const SedFunctionalRange& orig);


  /**
   * Assignment operator for SedFunctionalRange.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedFunctionalRange& operator=(const SedFunctionalRange& rhs);


  /**
   * Creates and returns a deep copy of this SedFunctionalRange object.
   *
   * @return a (deep) copy of this SedFunctionalRange object.
   */
  virtual SedFunctionalRange* clone() const;


  /**
   * Destructor for SedFunctionalRange.
   */
  virtual ~SedFunctionalRange();


  /**
   * Returns the value of the "range" attribute of this SedFunctionalRange.
   *
   * @return the value of the "range" attribute of this SedFunctionalRange as a string.
   */
  virtual const std::string& getRange() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedFunctionalRange's "range" attribute has been set.
   *
   * @return @c true if this SedFunctionalRange's "range" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetRange() const;


  /**
   * Sets the value of the "range" attribute of this SedFunctionalRange.
   *
   * @param range; const std::string& value of the "range" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setRange(const std::string& range);


  /**
   * Unsets the value of the "range" attribute of this SedFunctionalRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetRange();


  /**
   * Returns the "math" element of this SedFunctionalRange.
   *
   * @return the "math" element of this SedFunctionalRange.
   */
  virtual const ASTNode* getMath() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedFunctionalRange's "math" element has been set.
   *
   * @return @c true if this SedFunctionalRange's "math" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetMath() const;


  /**
   * Sets the "math" element of this SedFunctionalRange.
   *
   * @param math; ASTNode* determining the value of the "resultLevel" attribute to be set.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setMath(ASTNode* math);


  /**
   * Unsets the "math" element of this SedFunctionalRange.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetMath();


  /**
   * Returns the  "SedListOfVariables" in this SedFunctionalRange object.
   *
   * @return the "SedListOfVariables" attribute of this SedFunctionalRange.
   */
  const SedListOfVariables* getListOfVariables() const;


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedFunctionalRange.
   *
   * @see getNumVariables()
   */
  SedVariable* getVariable(unsigned int n);


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedFunctionalRange.
   *
   * @see getNumVariables()
   */
  const SedVariable* getVariable(unsigned int n) const;


  /**
   * Get a Variable from the SedListOfVariables
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Variable to get.
   *
   * @return the Variable in the SedListOfVariables
   * with the given id or NULL if no such
   * Variable exists.
   *
   * @see getVariable(unsigned int n)
   *
   * @see getNumVariables()
   */
  SedVariable* getVariable(const std::string& sid);


  /**
   * Get a Variable from the SedListOfVariables
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Variable to get.
   *
   * @return the Variable in the SedListOfVariables
   * with the given id or NULL if no such
   * Variable exists.
   *
   * @see getVariable(unsigned int n)
   *
   * @see getNumVariables()
   */
  const SedVariable* getVariable(const std::string& sid) const;


  /**
   * Adds a copy the given "SedVariable" to this SedFunctionalRange.
   *
   * @param sv; the SedVariable object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addVariable(const SedVariable* sv);


  /**
   * Get the number of SedVariable objects in this SedFunctionalRange.
   *
   * @return the number of SedVariable objects in this SedFunctionalRange
   */
  unsigned int getNumVariables() const;


  /**
   * Creates a new SedVariable object, adds it to this SedFunctionalRanges
   * SedListOfVariables and returns the SedVariable object created.
   *
   * @return a new SedVariable object instance
   *
   * @see addVariable(const SedVariable* sv)
   */
  SedVariable* createVariable();


  /**
   * Removes the nth Variable from the SedListOfVariables within this SedFunctionalRange.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Variable to remove.
   *
   * @see getNumVariables()
   */
  SedVariable* removeVariable(unsigned int n);


  /**
   * Removes the Variable with the given identifier from the SedListOfVariables within this SedFunctionalRange
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Variable to remove.
   *
   * @return the Variable removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedVariable* removeVariable(const std::string& sid);


  /**
   * Returns the  "SedListOfParameters" in this SedFunctionalRange object.
   *
   * @return the "SedListOfParameters" attribute of this SedFunctionalRange.
   */
  const SedListOfParameters* getListOfParameters() const;


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedFunctionalRange.
   *
   * @see getNumParameters()
   */
  SedParameter* getParameter(unsigned int n);


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedFunctionalRange.
   *
   * @see getNumParameters()
   */
  const SedParameter* getParameter(unsigned int n) const;


  /**
   * Get a Parameter from the SedListOfParameters
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Parameter to get.
   *
   * @return the Parameter in the SedListOfParameters
   * with the given id or NULL if no such
   * Parameter exists.
   *
   * @see getParameter(unsigned int n)
   *
   * @see getNumParameters()
   */
  SedParameter* getParameter(const std::string& sid);


  /**
   * Get a Parameter from the SedListOfParameters
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Parameter to get.
   *
   * @return the Parameter in the SedListOfParameters
   * with the given id or NULL if no such
   * Parameter exists.
   *
   * @see getParameter(unsigned int n)
   *
   * @see getNumParameters()
   */
  const SedParameter* getParameter(const std::string& sid) const;


  /**
   * Adds a copy the given "SedParameter" to this SedFunctionalRange.
   *
   * @param sp; the SedParameter object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addParameter(const SedParameter* sp);


  /**
   * Get the number of SedParameter objects in this SedFunctionalRange.
   *
   * @return the number of SedParameter objects in this SedFunctionalRange
   */
  unsigned int getNumParameters() const;


  /**
   * Creates a new SedParameter object, adds it to this SedFunctionalRanges
   * SedListOfParameters and returns the SedParameter object created.
   *
   * @return a new SedParameter object instance
   *
   * @see addParameter(const SedParameter* sp)
   */
  SedParameter* createParameter();


  /**
   * Removes the nth Parameter from the SedListOfParameters within this SedFunctionalRange.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Parameter to remove.
   *
   * @see getNumParameters()
   */
  SedParameter* removeParameter(unsigned int n);


  /**
   * Removes the Parameter with the given identifier from the SedListOfParameters within this SedFunctionalRange
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Parameter to remove.
   *
   * @return the Parameter removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedParameter* removeParameter(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedFunctionalRange, is
   * always @c "sedFunctionalRange".
   *
   * @return the name of this element, i.e. @c "sedFunctionalRange".
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
   * for this SedFunctionalRange object have been set.
   *
   * @note The required attributes for a SedFunctionalRange object are:
   * @li "range"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedFunctionalRange object have been set.
   *
   * @note The required elements for a SedFunctionalRange object are:
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

class LIBSEDML_EXTERN SedListOfFunctionalRanges : public SedListOf
{

public:

  /**
   * Creates a new SedListOfFunctionalRanges with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfFunctionalRanges
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfFunctionalRanges
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfFunctionalRanges
   */
  SedListOfFunctionalRanges(unsigned int level      = SEDML_DEFAULT_LEVEL,
                            unsigned int version    = SEDML_DEFAULT_VERSION
                           );


  /**
   * Creates a new SedListOfFunctionalRanges with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfFunctionalRanges(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfFunctionalRanges object.
   *
   * @return a (deep) copy of this SedListOfFunctionalRanges object.
   */
  virtual SedListOfFunctionalRanges* clone() const;


  /**
   * Get a FunctionalRange from the SedListOfFunctionalRanges.
   *
   * @param n the index number of the FunctionalRange to get.
   *
   * @return the nth FunctionalRange in this SedListOfFunctionalRanges.
   *
   * @see size()
   */
  virtual SedFunctionalRange* get(unsigned int n);


  /**
   * Get a FunctionalRange from the SedListOfFunctionalRanges.
   *
   * @param n the index number of the FunctionalRange to get.
   *
   * @return the nth FunctionalRange in this SedListOfFunctionalRanges.
   *
   * @see size()
   */
  virtual const SedFunctionalRange* get(unsigned int n) const;


  /**
   * Get a FunctionalRange from the SedListOfFunctionalRanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FunctionalRange to get.
   *
   * @return FunctionalRange in this SedListOfFunctionalRanges
   * with the given id or NULL if no such
   * FunctionalRange exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedFunctionalRange* get(const std::string& sid);


  /**
   * Get a FunctionalRange from the SedListOfFunctionalRanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the FunctionalRange to get.
   *
   * @return FunctionalRange in this SedListOfFunctionalRanges
   * with the given id or NULL if no such
   * FunctionalRange exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedFunctionalRange* get(const std::string& sid) const;


  /**
   * Adds a copy the given "FunctionalRange" to this SedListOfFunctionalRanges.
   *
   * @param fr; the FunctionalRange object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addFunctionalRange(const SedFunctionalRange* fr);


  /**
   * Get the number of FunctionalRange objects in this SedListOfFunctionalRanges.
   *
   * @return the number of FunctionalRange objects in this SedListOfFunctionalRanges
   */
  unsigned int getNumFunctionalRanges() const;


  /**
   * Creates a new FunctionalRange object, adds it to the
   * SedListOfFunctionalRanges and returns the FunctionalRange object created.
   *
   * @return a new FunctionalRange object instance
   *
   * @see addFunctionalRange(const SedFunctionalRange* fr)
   */
  SedFunctionalRange* createFunctionalRange();


  /**
   * Removes the nth FunctionalRange from this SedListOfFunctionalRanges
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the FunctionalRange to remove.
   *
   * @see size()
   */
  virtual SedFunctionalRange* remove(unsigned int n);


  /**
   * Removes the FunctionalRange from this SedListOfFunctionalRanges with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the FunctionalRange to remove.
   *
   * @return the FunctionalRange removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedFunctionalRange* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfFunctionalRanges, is
   * always @c "sedListOfFunctionalRanges".
   *
   * @return the name of this element, i.e. @c "sedListOfFunctionalRanges".
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
   * Creates a new FunctionalRange in this SedListOfFunctionalRanges
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
SedFunctionalRange_t *
SedFunctionalRange_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedFunctionalRange_free(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedFunctionalRange_t *
SedFunctionalRange_clone(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
char *
SedFunctionalRange_getRange(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
ASTNode_t*
SedFunctionalRange_getMath(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_isSetRange(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_isSetMath(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_setRange(SedFunctionalRange_t * sfr, const char * range);


LIBSEDML_EXTERN
int
SedFunctionalRange_setMath(SedFunctionalRange_t * sfr, ASTNode_t* math);


LIBSEDML_EXTERN
int
SedFunctionalRange_unsetRange(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_unsetMath(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_addVariable(SedFunctionalRange_t * sfr, SedVariable_t * sv);


LIBSEDML_EXTERN
SedVariable_t *
SedFunctionalRange_createVariable(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedListOf_t *
SedFunctionalRange_getSedListOfVariables(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedVariable_t *
SedFunctionalRange_getVariable(SedFunctionalRange_t * sfr, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedFunctionalRange_getVariableById(SedFunctionalRange_t * sfr, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedFunctionalRange_getNumVariables(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedVariable_t *
SedFunctionalRange_removeVariable(SedFunctionalRange_t * sfr, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedFunctionalRange_removeVariableById(SedFunctionalRange_t * sfr, const char * sid);


LIBSEDML_EXTERN
int
SedFunctionalRange_addParameter(SedFunctionalRange_t * sfr, SedParameter_t * sp);


LIBSEDML_EXTERN
SedParameter_t *
SedFunctionalRange_createParameter(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedListOf_t *
SedFunctionalRange_getSedListOfParameters(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedParameter_t *
SedFunctionalRange_getParameter(SedFunctionalRange_t * sfr, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedFunctionalRange_getParameterById(SedFunctionalRange_t * sfr, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedFunctionalRange_getNumParameters(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedParameter_t *
SedFunctionalRange_removeParameter(SedFunctionalRange_t * sfr, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedFunctionalRange_removeParameterById(SedFunctionalRange_t * sfr, const char * sid);


LIBSEDML_EXTERN
int
SedFunctionalRange_hasRequiredAttributes(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
int
SedFunctionalRange_hasRequiredElements(SedFunctionalRange_t * sfr);


LIBSEDML_EXTERN
SedFunctionalRange_t *
SedListOfFunctionalRanges_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedFunctionalRange_t *
SedListOfFunctionalRanges_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedFunctionalRange_H__  */

