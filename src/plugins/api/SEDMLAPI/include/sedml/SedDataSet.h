/**
 * @file:   SedDataSet.h
 * @brief:  Implementation of the SedDataSet class
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


#ifndef SedDataSet_H__
#define SedDataSet_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedDataSet : public SedBase
{

protected:

  std::string   mId;
  std::string   mLabel;
  std::string   mName;
  std::string   mDataReference;


public:

  /**
   * Creates a new SedDataSet with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedDataSet
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedDataSet
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedDataSet
   */
  SedDataSet(unsigned int level      = SEDML_DEFAULT_LEVEL,
             unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedDataSet with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedDataSet(SedNamespaces* sedns);


  /**
   * Copy constructor for SedDataSet.
   *
   * @param orig; the SedDataSet instance to copy.
   */
  SedDataSet(const SedDataSet& orig);


  /**
   * Assignment operator for SedDataSet.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedDataSet& operator=(const SedDataSet& rhs);


  /**
   * Creates and returns a deep copy of this SedDataSet object.
   *
   * @return a (deep) copy of this SedDataSet object.
   */
  virtual SedDataSet* clone() const;


  /**
   * Destructor for SedDataSet.
   */
  virtual ~SedDataSet();


  /**
   * Returns the value of the "id" attribute of this SedDataSet.
   *
   * @return the value of the "id" attribute of this SedDataSet as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataSet's "id" attribute has been set.
   *
   * @return @c true if this SedDataSet's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedDataSet.
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
   * Unsets the value of the "id" attribute of this SedDataSet.
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
   * Returns the value of the "label" attribute of this SedDataSet.
   *
   * @return the value of the "label" attribute of this SedDataSet as a string.
   */
  virtual const std::string& getLabel() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataSet's "label" attribute has been set.
   *
   * @return @c true if this SedDataSet's "label" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLabel() const;


  /**
   * Sets the value of the "label" attribute of this SedDataSet.
   *
   * @param label; const std::string& value of the "label" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLabel(const std::string& label);


  /**
   * Unsets the value of the "label" attribute of this SedDataSet.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLabel();


  /**
   * Returns the value of the "name" attribute of this SedDataSet.
   *
   * @return the value of the "name" attribute of this SedDataSet as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataSet's "name" attribute has been set.
   *
   * @return @c true if this SedDataSet's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedDataSet.
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
   * Unsets the value of the "name" attribute of this SedDataSet.
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
   * Returns the value of the "dataReference" attribute of this SedDataSet.
   *
   * @return the value of the "dataReference" attribute of this SedDataSet as a string.
   */
  virtual const std::string& getDataReference() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDataSet's "dataReference" attribute has been set.
   *
   * @return @c true if this SedDataSet's "dataReference" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetDataReference() const;


  /**
   * Sets the value of the "dataReference" attribute of this SedDataSet.
   *
   * @param dataReference; const std::string& value of the "dataReference" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setDataReference(const std::string& dataReference);


  /**
   * Unsets the value of the "dataReference" attribute of this SedDataSet.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetDataReference();


  /**
   * Returns the XML element name of this object, which for SedDataSet, is
   * always @c "sedDataSet".
   *
   * @return the name of this element, i.e. @c "sedDataSet".
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
   * for this SedDataSet object have been set.
   *
   * @note The required attributes for a SedDataSet object are:
   * @li "id"
   * @li "label"
   * @li "dataReference"
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

class LIBSEDML_EXTERN SedListOfDataSets : public SedListOf
{

public:

  /**
   * Creates a new SedListOfDataSets with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfDataSets
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfDataSets
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfDataSets
   */
  SedListOfDataSets(unsigned int level      = SEDML_DEFAULT_LEVEL,
                    unsigned int version    = SEDML_DEFAULT_VERSION
                   );


  /**
   * Creates a new SedListOfDataSets with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfDataSets(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfDataSets object.
   *
   * @return a (deep) copy of this SedListOfDataSets object.
   */
  virtual SedListOfDataSets* clone() const;


  /**
   * Get a DataSet from the SedListOfDataSets.
   *
   * @param n the index number of the DataSet to get.
   *
   * @return the nth DataSet in this SedListOfDataSets.
   *
   * @see size()
   */
  virtual SedDataSet* get(unsigned int n);


  /**
   * Get a DataSet from the SedListOfDataSets.
   *
   * @param n the index number of the DataSet to get.
   *
   * @return the nth DataSet in this SedListOfDataSets.
   *
   * @see size()
   */
  virtual const SedDataSet* get(unsigned int n) const;


  /**
   * Get a DataSet from the SedListOfDataSets
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataSet to get.
   *
   * @return DataSet in this SedListOfDataSets
   * with the given id or NULL if no such
   * DataSet exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedDataSet* get(const std::string& sid);


  /**
   * Get a DataSet from the SedListOfDataSets
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataSet to get.
   *
   * @return DataSet in this SedListOfDataSets
   * with the given id or NULL if no such
   * DataSet exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedDataSet* get(const std::string& sid) const;


  /**
   * Adds a copy the given "DataSet" to this SedListOfDataSets.
   *
   * @param ds; the DataSet object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addDataSet(const SedDataSet* ds);


  /**
   * Get the number of DataSet objects in this SedListOfDataSets.
   *
   * @return the number of DataSet objects in this SedListOfDataSets
   */
  unsigned int getNumDataSets() const;


  /**
   * Creates a new DataSet object, adds it to the
   * SedListOfDataSets and returns the DataSet object created.
   *
   * @return a new DataSet object instance
   *
   * @see addDataSet(const SedDataSet* ds)
   */
  SedDataSet* createDataSet();


  /**
   * Removes the nth DataSet from this SedListOfDataSets
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DataSet to remove.
   *
   * @see size()
   */
  virtual SedDataSet* remove(unsigned int n);


  /**
   * Removes the DataSet from this SedListOfDataSets with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the DataSet to remove.
   *
   * @return the DataSet removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedDataSet* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfDataSets, is
   * always @c "sedListOfDataSets".
   *
   * @return the name of this element, i.e. @c "sedListOfDataSets".
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
   * Creates a new DataSet in this SedListOfDataSets
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
SedDataSet_t *
SedDataSet_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedDataSet_free(SedDataSet_t * sds);


LIBSEDML_EXTERN
SedDataSet_t *
SedDataSet_clone(SedDataSet_t * sds);


LIBSEDML_EXTERN
char *
SedDataSet_getId(SedDataSet_t * sds);


LIBSEDML_EXTERN
char *
SedDataSet_getLabel(SedDataSet_t * sds);


LIBSEDML_EXTERN
char *
SedDataSet_getName(SedDataSet_t * sds);


LIBSEDML_EXTERN
char *
SedDataSet_getDataReference(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_isSetId(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_isSetLabel(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_isSetName(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_isSetDataReference(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_setId(SedDataSet_t * sds, const char * id);


LIBSEDML_EXTERN
int
SedDataSet_setLabel(SedDataSet_t * sds, const char * label);


LIBSEDML_EXTERN
int
SedDataSet_setName(SedDataSet_t * sds, const char * name);


LIBSEDML_EXTERN
int
SedDataSet_setDataReference(SedDataSet_t * sds, const char * dataReference);


LIBSEDML_EXTERN
int
SedDataSet_unsetId(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_unsetLabel(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_unsetName(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_unsetDataReference(SedDataSet_t * sds);


LIBSEDML_EXTERN
int
SedDataSet_hasRequiredAttributes(SedDataSet_t * sds);


LIBSEDML_EXTERN
SedDataSet_t *
SedListOfDataSets_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedDataSet_t *
SedListOfDataSets_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedDataSet_H__  */

