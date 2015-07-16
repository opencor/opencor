/**
 * @file:   SedDataGenerator.h
 * @brief:  Implementation of the SedDataGenerator class
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


#ifndef SedDataGenerator_H__
#define SedDataGenerator_H__


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


class LIBSEDML_EXTERN SedDataGenerator : public SedBase
{

protected:

  std::string   mId;
  std::string   mName;
  SedListOfVariables   mVariables;
  SedListOfParameters   mParameters;
  ASTNode*      mMath;


public:

  /**
   * Creates a new SedDataGenerator with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedDataGenerator
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedDataGenerator
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedDataGenerator
   */
  SedDataGenerator(unsigned int level      = SEDML_DEFAULT_LEVEL,
                   unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedDataGenerator with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedDataGenerator(SedNamespaces* sedns);


  /**
   * Copy constructor for SedDataGenerator.
   *
   * @param orig; the SedDataGenerator instance to copy.
   */
  SedDataGenerator(const SedDataGenerator& orig);


  /**
   * Assignment operator for SedDataGenerator.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedDataGenerator& operator=(const SedDataGenerator& rhs);


  /**
   * Creates and returns a deep copy of this SedDataGenerator object.
   *
   * @return a (deep) copy of this SedDataGenerator object.
   */
  virtual SedDataGenerator* clone() const;


  /**
   * Destructor for SedDataGenerator.
   */
  virtual ~SedDataGenerator();


  /**
   * Returns the value of the "id" attribute of this SedDataGenerator.
   *
   * @return the value of the "id" attribute of this SedDataGenerator as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataGenerator's "id" attribute has been set.
   *
   * @return @c true if this SedDataGenerator's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedDataGenerator.
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
   * Unsets the value of the "id" attribute of this SedDataGenerator.
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
   * Returns the value of the "name" attribute of this SedDataGenerator.
   *
   * @return the value of the "name" attribute of this SedDataGenerator as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataGenerator's "name" attribute has been set.
   *
   * @return @c true if this SedDataGenerator's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedDataGenerator.
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
   * Unsets the value of the "name" attribute of this SedDataGenerator.
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
   * Returns the "math" element of this SedDataGenerator.
   *
   * @return the "math" element of this SedDataGenerator.
   */
  virtual const ASTNode* getMath() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataGenerator's "math" element has been set.
   *
   * @return @c true if this SedDataGenerator's "math" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetMath() const;


  /**
   * Sets the "math" element of this SedDataGenerator.
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
   * Unsets the "math" element of this SedDataGenerator.
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
   * Returns the  "SedListOfVariables" in this SedDataGenerator object.
   *
   * @return the "SedListOfVariables" attribute of this SedDataGenerator.
   */
  const SedListOfVariables* getListOfVariables() const;


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedDataGenerator.
   *
   * @see getNumVariables()
   */
  SedVariable* getVariable(unsigned int n);


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in the SedListOfVariables within this SedDataGenerator.
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
   * Adds a copy the given "SedVariable" to this SedDataGenerator.
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
   * Get the number of SedVariable objects in this SedDataGenerator.
   *
   * @return the number of SedVariable objects in this SedDataGenerator
   */
  unsigned int getNumVariables() const;


  /**
   * Creates a new SedVariable object, adds it to this SedDataGenerators
   * SedListOfVariables and returns the SedVariable object created.
   *
   * @return a new SedVariable object instance
   *
   * @see addVariable(const SedVariable* sv)
   */
  SedVariable* createVariable();


  /**
   * Removes the nth Variable from the SedListOfVariables within this SedDataGenerator.
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
   * Removes the Variable with the given identifier from the SedListOfVariables within this SedDataGenerator
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
   * Returns the  "SedListOfParameters" in this SedDataGenerator object.
   *
   * @return the "SedListOfParameters" attribute of this SedDataGenerator.
   */
  const SedListOfParameters* getListOfParameters() const;


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedDataGenerator.
   *
   * @see getNumParameters()
   */
  SedParameter* getParameter(unsigned int n);


  /**
   * Get a Parameter from the SedListOfParameters.
   *
   * @param n the index number of the Parameter to get.
   *
   * @return the nth Parameter in the SedListOfParameters within this SedDataGenerator.
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
   * Adds a copy the given "SedParameter" to this SedDataGenerator.
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
   * Get the number of SedParameter objects in this SedDataGenerator.
   *
   * @return the number of SedParameter objects in this SedDataGenerator
   */
  unsigned int getNumParameters() const;


  /**
   * Creates a new SedParameter object, adds it to this SedDataGenerators
   * SedListOfParameters and returns the SedParameter object created.
   *
   * @return a new SedParameter object instance
   *
   * @see addParameter(const SedParameter* sp)
   */
  SedParameter* createParameter();


  /**
   * Removes the nth Parameter from the SedListOfParameters within this SedDataGenerator.
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
   * Removes the Parameter with the given identifier from the SedListOfParameters within this SedDataGenerator
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
   * Returns the XML element name of this object, which for SedDataGenerator, is
   * always @c "sedDataGenerator".
   *
   * @return the name of this element, i.e. @c "sedDataGenerator".
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
   * for this SedDataGenerator object have been set.
   *
   * @note The required attributes for a SedDataGenerator object are:
   * @li "id"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedDataGenerator object have been set.
   *
   * @note The required elements for a SedDataGenerator object are:
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

class LIBSEDML_EXTERN SedListOfDataGenerators : public SedListOf
{

public:

  /**
   * Creates a new SedListOfDataGenerators with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfDataGenerators
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfDataGenerators
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfDataGenerators
   */
  SedListOfDataGenerators(unsigned int level      = SEDML_DEFAULT_LEVEL,
                          unsigned int version    = SEDML_DEFAULT_VERSION
                         );


  /**
   * Creates a new SedListOfDataGenerators with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfDataGenerators(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfDataGenerators object.
   *
   * @return a (deep) copy of this SedListOfDataGenerators object.
   */
  virtual SedListOfDataGenerators* clone() const;


  /**
   * Get a DataGenerator from the SedListOfDataGenerators.
   *
   * @param n the index number of the DataGenerator to get.
   *
   * @return the nth DataGenerator in this SedListOfDataGenerators.
   *
   * @see size()
   */
  virtual SedDataGenerator* get(unsigned int n);


  /**
   * Get a DataGenerator from the SedListOfDataGenerators.
   *
   * @param n the index number of the DataGenerator to get.
   *
   * @return the nth DataGenerator in this SedListOfDataGenerators.
   *
   * @see size()
   */
  virtual const SedDataGenerator* get(unsigned int n) const;


  /**
   * Get a DataGenerator from the SedListOfDataGenerators
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataGenerator to get.
   *
   * @return DataGenerator in this SedListOfDataGenerators
   * with the given id or NULL if no such
   * DataGenerator exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedDataGenerator* get(const std::string& sid);


  /**
   * Get a DataGenerator from the SedListOfDataGenerators
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataGenerator to get.
   *
   * @return DataGenerator in this SedListOfDataGenerators
   * with the given id or NULL if no such
   * DataGenerator exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedDataGenerator* get(const std::string& sid) const;


  /**
   * Adds a copy the given "DataGenerator" to this SedListOfDataGenerators.
   *
   * @param dg; the DataGenerator object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addDataGenerator(const SedDataGenerator* dg);


  /**
   * Get the number of DataGenerator objects in this SedListOfDataGenerators.
   *
   * @return the number of DataGenerator objects in this SedListOfDataGenerators
   */
  unsigned int getNumDataGenerators() const;


  /**
   * Creates a new DataGenerator object, adds it to the
   * SedListOfDataGenerators and returns the DataGenerator object created.
   *
   * @return a new DataGenerator object instance
   *
   * @see addDataGenerator(const SedDataGenerator* dg)
   */
  SedDataGenerator* createDataGenerator();


  /**
   * Removes the nth DataGenerator from this SedListOfDataGenerators
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DataGenerator to remove.
   *
   * @see size()
   */
  virtual SedDataGenerator* remove(unsigned int n);


  /**
   * Removes the DataGenerator from this SedListOfDataGenerators with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the DataGenerator to remove.
   *
   * @return the DataGenerator removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedDataGenerator* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfDataGenerators, is
   * always @c "sedListOfDataGenerators".
   *
   * @return the name of this element, i.e. @c "sedListOfDataGenerators".
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
   * Creates a new DataGenerator in this SedListOfDataGenerators
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
SedDataGenerator_t *
SedDataGenerator_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedDataGenerator_free(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDataGenerator_clone(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
char *
SedDataGenerator_getId(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
char *
SedDataGenerator_getName(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
ASTNode_t*
SedDataGenerator_getMath(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_isSetId(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_isSetName(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_isSetMath(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_setId(SedDataGenerator_t * sdg, const char * id);


LIBSEDML_EXTERN
int
SedDataGenerator_setName(SedDataGenerator_t * sdg, const char * name);


LIBSEDML_EXTERN
int
SedDataGenerator_setMath(SedDataGenerator_t * sdg, ASTNode_t* math);


LIBSEDML_EXTERN
int
SedDataGenerator_unsetId(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_unsetName(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_unsetMath(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_addVariable(SedDataGenerator_t * sdg, SedVariable_t * sv);


LIBSEDML_EXTERN
SedVariable_t *
SedDataGenerator_createVariable(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedListOf_t *
SedDataGenerator_getSedListOfVariables(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedVariable_t *
SedDataGenerator_getVariable(SedDataGenerator_t * sdg, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedDataGenerator_getVariableById(SedDataGenerator_t * sdg, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDataGenerator_getNumVariables(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedVariable_t *
SedDataGenerator_removeVariable(SedDataGenerator_t * sdg, unsigned int n);


LIBSEDML_EXTERN
SedVariable_t *
SedDataGenerator_removeVariableById(SedDataGenerator_t * sdg, const char * sid);


LIBSEDML_EXTERN
int
SedDataGenerator_addParameter(SedDataGenerator_t * sdg, SedParameter_t * sp);


LIBSEDML_EXTERN
SedParameter_t *
SedDataGenerator_createParameter(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedListOf_t *
SedDataGenerator_getSedListOfParameters(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedParameter_t *
SedDataGenerator_getParameter(SedDataGenerator_t * sdg, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedDataGenerator_getParameterById(SedDataGenerator_t * sdg, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDataGenerator_getNumParameters(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedParameter_t *
SedDataGenerator_removeParameter(SedDataGenerator_t * sdg, unsigned int n);


LIBSEDML_EXTERN
SedParameter_t *
SedDataGenerator_removeParameterById(SedDataGenerator_t * sdg, const char * sid);


LIBSEDML_EXTERN
int
SedDataGenerator_hasRequiredAttributes(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
int
SedDataGenerator_hasRequiredElements(SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedListOfDataGenerators_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedListOfDataGenerators_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedDataGenerator_H__  */

