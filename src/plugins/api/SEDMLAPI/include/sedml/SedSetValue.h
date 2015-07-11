/**
 * @file:   SedSetValue.h
 * @brief:  Implementation of the SedSetValue class
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


#ifndef SedSetValue_H__
#define SedSetValue_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedVariable.h>
#include <sedml/SedParameter.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedSetValue : public SedBase
{

protected:

  SedListOfVariables   mVariables;
  SedListOfParameters   mParameters;
  std::string   mRange;
  std::string   mModelReference;
  std::string   mSymbol;
  std::string   mTarget;
  ASTNode*      mMath;


public:

  /**
   * Creates a new SedSetValue with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedSetValue
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedSetValue
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedSetValue
   */
  SedSetValue(unsigned int level      = SEDML_DEFAULT_LEVEL,
              unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedSetValue with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedSetValue(SedNamespaces* sedns);


  /**
   * Copy constructor for SedSetValue.
   *
   * @param orig; the SedSetValue instance to copy.
   */
  SedSetValue(const SedSetValue& orig);


  /**
   * Assignment operator for SedSetValue.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedSetValue& operator=(const SedSetValue& rhs);


  /**
   * Creates and returns a deep copy of this SedSetValue object.
   *
   * @return a (deep) copy of this SedSetValue object.
   */
  virtual SedSetValue* clone() const;


  /**
   * Destructor for SedSetValue.
   */
  virtual ~SedSetValue();


  /**
   * Returns the value of the "range" attribute of this SedSetValue.
   *
   * @return the value of the "range" attribute of this SedSetValue as a string.
   */
  virtual const std::string& getRange() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSetValue's "range" attribute has been set.
   *
   * @return @c true if this SedSetValue's "range" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetRange() const;


  /**
   * Sets the value of the "range" attribute of this SedSetValue.
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
   * Unsets the value of the "range" attribute of this SedSetValue.
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
   * Returns the value of the "modelReference" attribute of this SedSetValue.
   *
   * @return the value of the "modelReference" attribute of this SedSetValue as a string.
   */
  virtual const std::string& getModelReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSetValue's "modelReference" attribute has been set.
   *
   * @return @c true if this SedSetValue's "modelReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetModelReference() const;


  /**
   * Sets the value of the "modelReference" attribute of this SedSetValue.
   *
   * @param modelReference; const std::string& value of the "modelReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setModelReference(const std::string& modelReference);


  /**
   * Unsets the value of the "modelReference" attribute of this SedSetValue.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetModelReference();


  /**
   * Returns the value of the "symbol" attribute of this SedSetValue.
   *
   * @return the value of the "symbol" attribute of this SedSetValue as a string.
   */
  virtual const std::string& getSymbol() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSetValue's "symbol" attribute has been set.
   *
   * @return @c true if this SedSetValue's "symbol" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetSymbol() const;


  /**
   * Sets the value of the "symbol" attribute of this SedSetValue.
   *
   * @param symbol; const std::string& value of the "symbol" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setSymbol(const std::string& symbol);


  /**
   * Unsets the value of the "symbol" attribute of this SedSetValue.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetSymbol();


  /**
   * Returns the value of the "target" attribute of this SedSetValue.
   *
   * @return the value of the "target" attribute of this SedSetValue as a string.
   */
  virtual const std::string& getTarget() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSetValue's "target" attribute has been set.
   *
   * @return @c true if this SedSetValue's "target" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetTarget() const;


  /**
   * Sets the value of the "target" attribute of this SedSetValue.
   *
   * @param target; const std::string& value of the "target" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setTarget(const std::string& target);


  /**
   * Unsets the value of the "target" attribute of this SedSetValue.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetTarget();


  /**
   * Returns the "math" element of this SedSetValue.
   *
   * @return the "math" element of this SedSetValue.
   */
  virtual const ASTNode* getMath() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSetValue's "math" element has been set.
   *
   * @return @c true if this SedSetValue's "math" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetMath() const;


  /**
   * Sets the "math" element of this SedSetValue.
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
   * Unsets the "math" element of this SedSetValue.
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
   * Returns the  "SedListOfVariables" in this SedSetValue object.
   *
   * @return the "SedListOfVariables" attribute of this SedSetValue.
   */
  const SedListOfVariables* getListOfVariables() const;


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedSetValue.
   *
   * @see getNumVariables()
   */
  SedVariable* getVariable(unsigned int n);


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedSetValue.
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
   * Adds a copy the given "SedVariable" to this SedSetValue.
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
   * Get the number of SedVariable objects in this SedSetValue.
   *
   * @return the number of SedVariable objects in this SedSetValue
   */
  unsigned int getNumVariables() const;


  /**
   * Creates a new SedVariable object, adds it to this SedSetValues
   * SedListOfVariables and returns the SedVariable object created.
   *
   * @return a new SedVariable object instance
   *
   * @see addVariable(const SedVariable* sv)
   */
  SedVariable* createVariable();


  /**
   * Removes the nth Variable from the SedListOfVariables within this SedSetValue.
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
   * Removes the Variable with the given identifier from the SedListOfVariables within this SedSetValue
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
   * Returns the  "SedListOfParameters" in this SedSetValue object.
   *
   * @return the "SedListOfParameters" attribute of this SedSetValue.
   */
  const SedListOfParameters* getListOfParameters() const;


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedSetValue.
   *
   * @see getNumParameters()
   */
  SedParameter* getParameter(unsigned int n);


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedSetValue.
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
   * Adds a copy the given "SedParameter" to this SedSetValue.
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
   * Get the number of SedParameter objects in this SedSetValue.
   *
   * @return the number of SedParameter objects in this SedSetValue
   */
  unsigned int getNumParameters() const;


  /**
   * Creates a new SedParameter object, adds it to this SedSetValues
   * SedListOfParameters and returns the SedParameter object created.
   *
   * @return a new SedParameter object instance
   *
   * @see addParameter(const SedParameter* sp)
   */
  SedParameter* createParameter();


  /**
   * Removes the nth Parameter from the SedListOfParameters within this SedSetValue.
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
   * Removes the Parameter with the given identifier from the SedListOfParameters within this SedSetValue
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
   * Returns the XML element name of this object, which for SedSetValue, is
   * always @c "sedSetValue".
   *
   * @return the name of this element, i.e. @c "sedSetValue".
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
   * for this SedSetValue object have been set.
   *
   * @note The required attributes for a SedSetValue object are:
   * @li "modelReference"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedSetValue object have been set.
   *
   * @note The required elements for a SedSetValue object are:
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

class LIBSEDML_EXTERN SedListOfTaskChanges : public SedListOf
{

public:

  /**
   * Creates a new SedListOfTaskChanges with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfTaskChanges
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfTaskChanges
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfTaskChanges
   */
  SedListOfTaskChanges(unsigned int level      = SEDML_DEFAULT_LEVEL,
                       unsigned int version    = SEDML_DEFAULT_VERSION
                      );


  /**
   * Creates a new SedListOfTaskChanges with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfTaskChanges(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfTaskChanges object.
   *
   * @return a (deep) copy of this SedListOfTaskChanges object.
   */
  virtual SedListOfTaskChanges* clone() const;


  /**
   * Get a SetValue from the SedListOfTaskChanges.
   *
   * @param n the index number of the SetValue to get.
   *
   * @return the nth SetValue in this SedListOfTaskChanges.
   *
   * @see size()
   */
  virtual SedSetValue* get(unsigned int n);


  /**
   * Get a SetValue from the SedListOfTaskChanges.
   *
   * @param n the index number of the SetValue to get.
   *
   * @return the nth SetValue in this SedListOfTaskChanges.
   *
   * @see size()
   */
  virtual const SedSetValue* get(unsigned int n) const;


  /**
   * Get a SetValue from the SedListOfTaskChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SetValue to get.
   *
   * @return SetValue in this SedListOfTaskChanges
   * with the given id or NULL if no such
   * SetValue exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedSetValue* get(const std::string& sid);


  /**
   * Get a SetValue from the SedListOfTaskChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the SetValue to get.
   *
   * @return SetValue in this SedListOfTaskChanges
   * with the given id or NULL if no such
   * SetValue exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedSetValue* get(const std::string& sid) const;


  /**
   * Adds a copy the given "SetValue" to this SedListOfTaskChanges.
   *
   * @param sv; the SetValue object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addSetValue(const SedSetValue* sv);


  /**
   * Get the number of SetValue objects in this SedListOfTaskChanges.
   *
   * @return the number of SetValue objects in this SedListOfTaskChanges
   */
  unsigned int getNumSetValues() const;


  /**
   * Creates a new SetValue object, adds it to the
   * SedListOfTaskChanges and returns the SetValue object created.
   *
   * @return a new SetValue object instance
   *
   * @see addSetValue(const SedSetValue* sv)
   */
  SedSetValue* createSetValue();


  /**
   * Removes the nth SetValue from this SedListOfTaskChanges
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the SetValue to remove.
   *
   * @see size()
   */
  virtual SedSetValue* remove(unsigned int n);


  /**
   * Removes the SetValue from this SedListOfTaskChanges with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the SetValue to remove.
   *
   * @return the SetValue removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedSetValue* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfTaskChanges, is
   * always @c "sedListOfTaskChanges".
   *
   * @return the name of this element, i.e. @c "sedListOfTaskChanges".
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
   * Creates a new SetValue in this SedListOfTaskChanges
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
SedSetValue_t *
SedSetValue_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedSetValue_free(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedSetValue_t *
SedSetValue_clone(SedSetValue_t * ssv);


LIBSEDML_EXTERN
char *
SedSetValue_getRange(SedSetValue_t * ssv);


LIBSEDML_EXTERN
char *
SedSetValue_getModelReference(SedSetValue_t * ssv);


LIBSEDML_EXTERN
char *
SedSetValue_getSymbol(SedSetValue_t * ssv);


LIBSEDML_EXTERN
char *
SedSetValue_getTarget(SedSetValue_t * ssv);


LIBSEDML_EXTERN
ASTNode_t*
SedSetValue_getMath(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_isSetRange(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_isSetModelReference(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_isSetSymbol(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_isSetTarget(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_isSetMath(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_setRange(SedSetValue_t * ssv, const char * range);


LIBSEDML_EXTERN
int
SedSetValue_setModelReference(SedSetValue_t * ssv, const char * modelReference);


LIBSEDML_EXTERN
int
SedSetValue_setSymbol(SedSetValue_t * ssv, const char * symbol);


LIBSEDML_EXTERN
int
SedSetValue_setTarget(SedSetValue_t * ssv, const char * target);


LIBSEDML_EXTERN
int
SedSetValue_setMath(SedSetValue_t * ssv, ASTNode_t* math);


LIBSEDML_EXTERN
int
SedSetValue_unsetRange(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_unsetModelReference(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_unsetSymbol(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_unsetTarget(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_unsetMath(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_addVariable(SedSetValue_t * ssv, SedVariable_t * sv);


LIBSEDML_EXTERN
SedVariable_t *
SedSetValue_createVariable(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedListOf_t *
SedSetValue_getSedListOfVariables(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedVariable_t *
SedSetValue_getVariable(SedSetValue_t * ssv, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedSetValue_getVariableById(SedSetValue_t * ssv, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedSetValue_getNumVariables(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedVariable_t *
SedSetValue_removeVariable(SedSetValue_t * ssv, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedSetValue_removeVariableById(SedSetValue_t * ssv, const char * sid);


LIBSEDML_EXTERN
int
SedSetValue_addParameter(SedSetValue_t * ssv, SedParameter_t * sp);


LIBSEDML_EXTERN
SedParameter_t *
SedSetValue_createParameter(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedListOf_t *
SedSetValue_getSedListOfParameters(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedParameter_t *
SedSetValue_getParameter(SedSetValue_t * ssv, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedSetValue_getParameterById(SedSetValue_t * ssv, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedSetValue_getNumParameters(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedParameter_t *
SedSetValue_removeParameter(SedSetValue_t * ssv, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedSetValue_removeParameterById(SedSetValue_t * ssv, const char * sid);


LIBSEDML_EXTERN
int
SedSetValue_hasRequiredAttributes(SedSetValue_t * ssv);


LIBSEDML_EXTERN
int
SedSetValue_hasRequiredElements(SedSetValue_t * ssv);


LIBSEDML_EXTERN
SedSetValue_t *
SedListOfTaskChanges_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedSetValue_t *
SedListOfTaskChanges_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedSetValue_H__  */

