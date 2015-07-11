/**
 * @file:   SedModel.h
 * @brief:  Implementation of the SedModel class
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


#ifndef SedModel_H__
#define SedModel_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedChange.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedModel : public SedBase
{

protected:

  std::string   mId;
  std::string   mName;
  std::string   mLanguage;
  std::string   mSource;
  SedListOfChanges   mChanges;


public:

  /**
   * Creates a new SedModel with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedModel
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedModel
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedModel
   */
  SedModel(unsigned int level      = SEDML_DEFAULT_LEVEL,
           unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedModel with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedModel(SedNamespaces* sedns);


  /**
   * Copy constructor for SedModel.
   *
   * @param orig; the SedModel instance to copy.
   */
  SedModel(const SedModel& orig);


  /**
   * Assignment operator for SedModel.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedModel& operator=(const SedModel& rhs);


  /**
   * Creates and returns a deep copy of this SedModel object.
   *
   * @return a (deep) copy of this SedModel object.
   */
  virtual SedModel* clone() const;


  /**
   * Destructor for SedModel.
   */
  virtual ~SedModel();


  /**
   * Returns the value of the "id" attribute of this SedModel.
   *
   * @return the value of the "id" attribute of this SedModel as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedModel's "id" attribute has been set.
   *
   * @return @c true if this SedModel's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedModel.
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
   * Unsets the value of the "id" attribute of this SedModel.
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
   * Returns the value of the "name" attribute of this SedModel.
   *
   * @return the value of the "name" attribute of this SedModel as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedModel's "name" attribute has been set.
   *
   * @return @c true if this SedModel's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedModel.
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
   * Unsets the value of the "name" attribute of this SedModel.
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
   * Returns the value of the "language" attribute of this SedModel.
   *
   * @return the value of the "language" attribute of this SedModel as a string.
   */
  virtual const std::string& getLanguage() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedModel's "language" attribute has been set.
   *
   * @return @c true if this SedModel's "language" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLanguage() const;


  /**
   * Sets the value of the "language" attribute of this SedModel.
   *
   * @param language; const std::string& value of the "language" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLanguage(const std::string& language);


  /**
   * Unsets the value of the "language" attribute of this SedModel.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLanguage();


  /**
   * Returns the value of the "source" attribute of this SedModel.
   *
   * @return the value of the "source" attribute of this SedModel as a string.
   */
  virtual const std::string& getSource() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedModel's "source" attribute has been set.
   *
   * @return @c true if this SedModel's "source" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetSource() const;


  /**
   * Sets the value of the "source" attribute of this SedModel.
   *
   * @param source; const std::string& value of the "source" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setSource(const std::string& source);


  /**
   * Unsets the value of the "source" attribute of this SedModel.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetSource();


  /**
   * Returns the  "SedListOfChanges" in this SedModel object.
   *
   * @return the "SedListOfChanges" attribute of this SedModel.
   */
  const SedListOfChanges* getListOfChanges() const;


  /**
   * Get a Change from the SedListOfChanges.
   *
   * @param n the index number of the Change to get.
   *
   * @return the nth Change in the SedListOfChanges within this SedModel.
   *
   * @see getNumChanges()
   */
  SedChange* getChange(unsigned int n);


  /**
   * Get a Change from the SedListOfChanges.
   *
   * @param n the index number of the Change to get.
   *
   * @return the nth Change in the SedListOfChanges within this SedModel.
   *
   * @see getNumChanges()
   */
  const SedChange* getChange(unsigned int n) const;


  /**
   * Get a Change from the SedListOfChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Change to get.
   *
   * @return the Change in the SedListOfChanges
   * with the given id or NULL if no such
   * Change exists.
   *
   * @see getChange(unsigned int n)
   *
   * @see getNumChanges()
   */
  SedChange* getChange(const std::string& sid);


  /**
   * Get a Change from the SedListOfChanges
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Change to get.
   *
   * @return the Change in the SedListOfChanges
   * with the given id or NULL if no such
   * Change exists.
   *
   * @see getChange(unsigned int n)
   *
   * @see getNumChanges()
   */
  const SedChange* getChange(const std::string& sid) const;


  /**
   * Adds a copy the given "SedChange" to this SedModel.
   *
   * @param sc; the SedChange object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addChange(const SedChange* sc);


  /**
   * Get the number of SedChange objects in this SedModel.
   *
   * @return the number of SedChange objects in this SedModel
   */
  unsigned int getNumChanges() const;


  /**
   * Creates a new SedRemoveXML object, adds it to this SedModels
   * SedListOfChanges and returns the SedRemoveXML object created.
   *
   * @return a new SedRemoveXML object instance
   *
   * @see addSedChange(const SedChange* sc)
   */
  SedRemoveXML* createRemoveXML();


  /**
   * Creates a new SedAddXML object, adds it to this SedModels
   * SedListOfChanges and returns the SedAddXML object created.
   *
   * @return a new SedAddXML object instance
   *
   * @see addSedChange(const SedChange* sc)
   */
  SedAddXML* createAddXML();


  /**
   * Creates a new SedChangeXML object, adds it to this SedModels
   * SedListOfChanges and returns the SedChangeXML object created.
   *
   * @return a new SedChangeXML object instance
   *
   * @see addSedChange(const SedChange* sc)
   */
  SedChangeXML* createChangeXML();


  /**
   * Creates a new SedChangeAttribute object, adds it to this SedModels
   * SedListOfChanges and returns the SedChangeAttribute object created.
   *
   * @return a new SedChangeAttribute object instance
   *
   * @see addSedChange(const SedChange* sc)
   */
  SedChangeAttribute* createChangeAttribute();


  /**
   * Creates a new SedComputeChange object, adds it to this SedModels
   * SedListOfChanges and returns the SedComputeChange object created.
   *
   * @return a new SedComputeChange object instance
   *
   * @see addSedChange(const SedChange* sc)
   */
  SedComputeChange* createComputeChange();


  /**
   * Removes the nth Change from the SedListOfChanges within this SedModel.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Change to remove.
   *
   * @see getNumChanges()
   */
  SedChange* removeChange(unsigned int n);


  /**
   * Removes the Change with the given identifier from the SedListOfChanges within this SedModel
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Change to remove.
   *
   * @return the Change removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedChange* removeChange(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedModel, is
   * always @c "sedModel".
   *
   * @return the name of this element, i.e. @c "sedModel".
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
   * for this SedModel object have been set.
   *
   * @note The required attributes for a SedModel object are:
   * @li "id"
   * @li "source"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedModel object have been set.
   *
   * @note The required elements for a SedModel object are:
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

class LIBSEDML_EXTERN SedListOfModels : public SedListOf
{

public:

  /**
   * Creates a new SedListOfModels with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfModels
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfModels
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfModels
   */
  SedListOfModels(unsigned int level      = SEDML_DEFAULT_LEVEL,
                  unsigned int version    = SEDML_DEFAULT_VERSION
                 );


  /**
   * Creates a new SedListOfModels with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfModels(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfModels object.
   *
   * @return a (deep) copy of this SedListOfModels object.
   */
  virtual SedListOfModels* clone() const;


  /**
   * Get a Model from the SedListOfModels.
   *
   * @param n the index number of the Model to get.
   *
   * @return the nth Model in this SedListOfModels.
   *
   * @see size()
   */
  virtual SedModel* get(unsigned int n);


  /**
   * Get a Model from the SedListOfModels.
   *
   * @param n the index number of the Model to get.
   *
   * @return the nth Model in this SedListOfModels.
   *
   * @see size()
   */
  virtual const SedModel* get(unsigned int n) const;


  /**
   * Get a Model from the SedListOfModels
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Model to get.
   *
   * @return Model in this SedListOfModels
   * with the given id or NULL if no such
   * Model exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedModel* get(const std::string& sid);


  /**
   * Get a Model from the SedListOfModels
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Model to get.
   *
   * @return Model in this SedListOfModels
   * with the given id or NULL if no such
   * Model exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedModel* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Model" to this SedListOfModels.
   *
   * @param m; the Model object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addModel(const SedModel* m);


  /**
   * Get the number of Model objects in this SedListOfModels.
   *
   * @return the number of Model objects in this SedListOfModels
   */
  unsigned int getNumModels() const;


  /**
   * Creates a new Model object, adds it to the
   * SedListOfModels and returns the Model object created.
   *
   * @return a new Model object instance
   *
   * @see addModel(const SedModel* m)
   */
  SedModel* createModel();


  /**
   * Removes the nth Model from this SedListOfModels
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Model to remove.
   *
   * @see size()
   */
  virtual SedModel* remove(unsigned int n);


  /**
   * Removes the Model from this SedListOfModels with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Model to remove.
   *
   * @return the Model removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedModel* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfModels, is
   * always @c "sedListOfModels".
   *
   * @return the name of this element, i.e. @c "sedListOfModels".
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
   * Creates a new Model in this SedListOfModels
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
SedModel_t *
SedModel_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedModel_free(SedModel_t * sm);


LIBSEDML_EXTERN
SedModel_t *
SedModel_clone(SedModel_t * sm);


LIBSEDML_EXTERN
char *
SedModel_getId(SedModel_t * sm);


LIBSEDML_EXTERN
char *
SedModel_getName(SedModel_t * sm);


LIBSEDML_EXTERN
char *
SedModel_getLanguage(SedModel_t * sm);


LIBSEDML_EXTERN
char *
SedModel_getSource(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_isSetId(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_isSetName(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_isSetLanguage(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_isSetSource(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_setId(SedModel_t * sm, const char * id);


LIBSEDML_EXTERN
int
SedModel_setName(SedModel_t * sm, const char * name);


LIBSEDML_EXTERN
int
SedModel_setLanguage(SedModel_t * sm, const char * language);


LIBSEDML_EXTERN
int
SedModel_setSource(SedModel_t * sm, const char * source);


LIBSEDML_EXTERN
int
SedModel_unsetId(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_unsetName(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_unsetLanguage(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_unsetSource(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_addChange(SedModel_t * sm, SedChange_t * sc);


LIBSEDML_EXTERN
SedRemoveXML_t *
SedModel_createRemoveXML(SedModel_t * sm);


LIBSEDML_EXTERN
SedAddXML_t *
SedModel_createAddXML(SedModel_t * sm);


LIBSEDML_EXTERN
SedChangeXML_t *
SedModel_createChangeXML(SedModel_t * sm);


LIBSEDML_EXTERN
SedChangeAttribute_t *
SedModel_createChangeAttribute(SedModel_t * sm);


LIBSEDML_EXTERN
SedComputeChange_t *
SedModel_createComputeChange(SedModel_t * sm);


LIBSEDML_EXTERN
SedListOf_t *
SedModel_getSedListOfChanges(SedModel_t * sm);


LIBSEDML_EXTERN
SedChange_t *
SedModel_getChange(SedModel_t * sm, unsigned int n);


LIBSEDML_EXTERN
SedChange_t *
SedModel_getChangeById(SedModel_t * sm, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedModel_getNumChanges(SedModel_t * sm);


LIBSEDML_EXTERN
SedChange_t *
SedModel_removeChange(SedModel_t * sm, unsigned int n);


LIBSEDML_EXTERN
SedChange_t *
SedModel_removeChangeById(SedModel_t * sm, const char * sid);


LIBSEDML_EXTERN
int
SedModel_hasRequiredAttributes(SedModel_t * sm);


LIBSEDML_EXTERN
int
SedModel_hasRequiredElements(SedModel_t * sm);


LIBSEDML_EXTERN
SedModel_t *
SedListOfModels_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedModel_t *
SedListOfModels_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedModel_H__  */

