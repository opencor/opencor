/**
 * @file:   SedVariable.h
 * @brief:  Implementation of the SedVariable class
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


#ifndef SedVariable_H__
#define SedVariable_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedVariable : public SedBase
{

protected:

  std::string   mId;
  std::string   mName;
  std::string   mSymbol;
  std::string   mTarget;
  std::string   mTaskReference;
  std::string   mModelReference;


public:

  /**
   * Creates a new SedVariable with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedVariable
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedVariable
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedVariable
   */
  SedVariable(unsigned int level      = SEDML_DEFAULT_LEVEL,
              unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedVariable with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedVariable(SedNamespaces* sedns);


  /**
   * Copy constructor for SedVariable.
   *
   * @param orig; the SedVariable instance to copy.
   */
  SedVariable(const SedVariable& orig);


  /**
   * Assignment operator for SedVariable.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedVariable& operator=(const SedVariable& rhs);


  /**
   * Creates and returns a deep copy of this SedVariable object.
   *
   * @return a (deep) copy of this SedVariable object.
   */
  virtual SedVariable* clone() const;


  /**
   * Destructor for SedVariable.
   */
  virtual ~SedVariable();


  /**
   * Returns the value of the "id" attribute of this SedVariable.
   *
   * @return the value of the "id" attribute of this SedVariable as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "id" attribute has been set.
   *
   * @return @c true if this SedVariable's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedVariable.
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
   * Unsets the value of the "id" attribute of this SedVariable.
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
   * Returns the value of the "name" attribute of this SedVariable.
   *
   * @return the value of the "name" attribute of this SedVariable as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "name" attribute has been set.
   *
   * @return @c true if this SedVariable's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedVariable.
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
   * Unsets the value of the "name" attribute of this SedVariable.
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
   * Returns the value of the "symbol" attribute of this SedVariable.
   *
   * @return the value of the "symbol" attribute of this SedVariable as a string.
   */
  virtual const std::string& getSymbol() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "symbol" attribute has been set.
   *
   * @return @c true if this SedVariable's "symbol" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetSymbol() const;


  /**
   * Sets the value of the "symbol" attribute of this SedVariable.
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
   * Unsets the value of the "symbol" attribute of this SedVariable.
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
   * Returns the value of the "target" attribute of this SedVariable.
   *
   * @return the value of the "target" attribute of this SedVariable as a string.
   */
  virtual const std::string& getTarget() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "target" attribute has been set.
   *
   * @return @c true if this SedVariable's "target" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetTarget() const;


  /**
   * Sets the value of the "target" attribute of this SedVariable.
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
   * Unsets the value of the "target" attribute of this SedVariable.
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
   * Returns the value of the "taskReference" attribute of this SedVariable.
   *
   * @return the value of the "taskReference" attribute of this SedVariable as a string.
   */
  virtual const std::string& getTaskReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "taskReference" attribute has been set.
   *
   * @return @c true if this SedVariable's "taskReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetTaskReference() const;


  /**
   * Sets the value of the "taskReference" attribute of this SedVariable.
   *
   * @param taskReference; const std::string& value of the "taskReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setTaskReference(const std::string& taskReference);


  /**
   * Unsets the value of the "taskReference" attribute of this SedVariable.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetTaskReference();


  /**
   * Returns the value of the "modelReference" attribute of this SedVariable.
   *
   * @return the value of the "modelReference" attribute of this SedVariable as a string.
   */
  virtual const std::string& getModelReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedVariable's "modelReference" attribute has been set.
   *
   * @return @c true if this SedVariable's "modelReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetModelReference() const;


  /**
   * Sets the value of the "modelReference" attribute of this SedVariable.
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
   * Unsets the value of the "modelReference" attribute of this SedVariable.
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
   * Returns the XML element name of this object, which for SedVariable, is
   * always @c "sedVariable".
   *
   * @return the name of this element, i.e. @c "sedVariable".
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
   * for this SedVariable object have been set.
   *
   * @note The required attributes for a SedVariable object are:
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

class LIBSEDML_EXTERN SedListOfVariables : public SedListOf
{

public:

  /**
   * Creates a new SedListOfVariables with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfVariables
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfVariables
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfVariables
   */
  SedListOfVariables(unsigned int level      = SEDML_DEFAULT_LEVEL,
                     unsigned int version    = SEDML_DEFAULT_VERSION
                    );


  /**
   * Creates a new SedListOfVariables with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfVariables(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfVariables object.
   *
   * @return a (deep) copy of this SedListOfVariables object.
   */
  virtual SedListOfVariables* clone() const;


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in this SedListOfVariables.
   *
   * @see size()
   */
  virtual SedVariable* get(unsigned int n);


  /**
   * Get a Variable from the SedListOfVariables.
   *
   * @param n the index number of the Variable to get.
   *
   * @return the nth Variable in this SedListOfVariables.
   *
   * @see size()
   */
  virtual const SedVariable* get(unsigned int n) const;


  /**
   * Get a Variable from the SedListOfVariables
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Variable to get.
   *
   * @return Variable in this SedListOfVariables
   * with the given id or NULL if no such
   * Variable exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedVariable* get(const std::string& sid);


  /**
   * Get a Variable from the SedListOfVariables
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Variable to get.
   *
   * @return Variable in this SedListOfVariables
   * with the given id or NULL if no such
   * Variable exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedVariable* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Variable" to this SedListOfVariables.
   *
   * @param v; the Variable object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addVariable(const SedVariable* v);


  /**
   * Get the number of Variable objects in this SedListOfVariables.
   *
   * @return the number of Variable objects in this SedListOfVariables
   */
  unsigned int getNumVariables() const;


  /**
   * Creates a new Variable object, adds it to the
   * SedListOfVariables and returns the Variable object created.
   *
   * @return a new Variable object instance
   *
   * @see addVariable(const SedVariable* v)
   */
  SedVariable* createVariable();


  /**
   * Removes the nth Variable from this SedListOfVariables
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Variable to remove.
   *
   * @see size()
   */
  virtual SedVariable* remove(unsigned int n);


  /**
   * Removes the Variable from this SedListOfVariables with the given identifier
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
  virtual SedVariable* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfVariables, is
   * always @c "sedListOfVariables".
   *
   * @return the name of this element, i.e. @c "sedListOfVariables".
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
   * Creates a new Variable in this SedListOfVariables
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
SedVariable_t *
SedVariable_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedVariable_free(SedVariable_t * sv);


LIBSEDML_EXTERN
SedVariable_t *
SedVariable_clone(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getId(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getName(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getSymbol(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getTarget(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getTaskReference(SedVariable_t * sv);


LIBSEDML_EXTERN
char *
SedVariable_getModelReference(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetId(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetName(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetSymbol(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetTarget(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetTaskReference(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_isSetModelReference(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_setId(SedVariable_t * sv, const char * id);


LIBSEDML_EXTERN
int
SedVariable_setName(SedVariable_t * sv, const char * name);


LIBSEDML_EXTERN
int
SedVariable_setSymbol(SedVariable_t * sv, const char * symbol);


LIBSEDML_EXTERN
int
SedVariable_setTarget(SedVariable_t * sv, const char * target);


LIBSEDML_EXTERN
int
SedVariable_setTaskReference(SedVariable_t * sv, const char * taskReference);


LIBSEDML_EXTERN
int
SedVariable_setModelReference(SedVariable_t * sv, const char * modelReference);


LIBSEDML_EXTERN
int
SedVariable_unsetId(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_unsetName(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_unsetSymbol(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_unsetTarget(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_unsetTaskReference(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_unsetModelReference(SedVariable_t * sv);


LIBSEDML_EXTERN
int
SedVariable_hasRequiredAttributes(SedVariable_t * sv);


LIBSEDML_EXTERN
SedVariable_t *
SedListOfVariables_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedVariable_t *
SedListOfVariables_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedVariable_H__  */

