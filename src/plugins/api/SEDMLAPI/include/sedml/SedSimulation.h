/**
 * @file:   SedSimulation.h
 * @brief:  Implementation of the SedSimulation class
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


#ifndef SedSimulation_H__
#define SedSimulation_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedSimulation : public SedBase
{

protected:

  std::string   mId;
  std::string   mName;
  SedAlgorithm*      mAlgorithm;


public:

  /**
   * Creates a new SedSimulation with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedSimulation
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedSimulation
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedSimulation
   */
  SedSimulation(unsigned int level      = SEDML_DEFAULT_LEVEL,
                unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedSimulation with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedSimulation(SedNamespaces* sedns);


  /**
   * Copy constructor for SedSimulation.
   *
   * @param orig; the SedSimulation instance to copy.
   */
  SedSimulation(const SedSimulation& orig);


  /**
   * Assignment operator for SedSimulation.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedSimulation& operator=(const SedSimulation& rhs);


  /**
   * Creates and returns a deep copy of this SedSimulation object.
   *
   * @return a (deep) copy of this SedSimulation object.
   */
  virtual SedSimulation* clone() const;


  /**
   * Destructor for SedSimulation.
   */
  virtual ~SedSimulation();


  /**
   * Returns the value of the "id" attribute of this SedSimulation.
   *
   * @return the value of the "id" attribute of this SedSimulation as a string.
   */
  virtual const std::string& getId() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSimulation's "id" attribute has been set.
   *
   * @return @c true if this SedSimulation's "id" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetId() const;


  /**
   * Sets the value of the "id" attribute of this SedSimulation.
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
   * Unsets the value of the "id" attribute of this SedSimulation.
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
   * Returns the value of the "name" attribute of this SedSimulation.
   *
   * @return the value of the "name" attribute of this SedSimulation as a string.
   */
  virtual const std::string& getName() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSimulation's "name" attribute has been set.
   *
   * @return @c true if this SedSimulation's "name" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetName() const;


  /**
   * Sets the value of the "name" attribute of this SedSimulation.
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
   * Unsets the value of the "name" attribute of this SedSimulation.
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
   * Returns the "algorithm" element of this SedSimulation.
   *
   * @return the "algorithm" element of this SedSimulation.
   */
  virtual const SedAlgorithm* getAlgorithm() const;


  /**
   * Creates a new "SedAlgorithm" and sets it for this SedSimulation.
   *
   * @return the created "SedAlgorithm" element of this SedSimulation.
   */
  virtual SedAlgorithm* createAlgorithm();


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedSimulation's "algorithm" element has been set.
   *
   * @return @c true if this SedSimulation's "algorithm" element has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetAlgorithm() const;


  /**
   * Sets the "algorithm" element of this SedSimulation.
   *
   * @param algorithm; SedAlgorithm* determining the value of the "resultLevel" attribute to be set.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setAlgorithm(SedAlgorithm* algorithm);


  /**
   * Unsets the "algorithm" element of this SedSimulation.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetAlgorithm();


  /**
   * Returns the XML element name of this object, which for SedSimulation, is
   * always @c "sedSimulation".
   *
   * @return the name of this element, i.e. @c "sedSimulation".
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
   * for this SedSimulation object have been set.
   *
   * @note The required attributes for a SedSimulation object are:
   * @li "id"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedSimulation object have been set.
   *
   * @note The required elements for a SedSimulation object are:
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

class LIBSEDML_EXTERN SedListOfSimulations : public SedListOf
{

public:

  /**
   * Creates a new SedListOfSimulations with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedListOfSimulations
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedListOfSimulations
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedListOfSimulations
   */
  SedListOfSimulations(unsigned int level      = SEDML_DEFAULT_LEVEL,
                       unsigned int version    = SEDML_DEFAULT_VERSION
                      );


  /**
   * Creates a new SedListOfSimulations with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedListOfSimulations(SedNamespaces* sedns);


  /**
   * Creates and returns a deep copy of this SedListOfSimulations object.
   *
   * @return a (deep) copy of this SedListOfSimulations object.
   */
  virtual SedListOfSimulations* clone() const;


  /**
   * Get a Simulation from the SedListOfSimulations.
   *
   * @param n the index number of the Simulation to get.
   *
   * @return the nth Simulation in this SedListOfSimulations.
   *
   * @see size()
   */
  virtual SedSimulation* get(unsigned int n);


  /**
   * Get a Simulation from the SedListOfSimulations.
   *
   * @param n the index number of the Simulation to get.
   *
   * @return the nth Simulation in this SedListOfSimulations.
   *
   * @see size()
   */
  virtual const SedSimulation* get(unsigned int n) const;


  /**
   * Get a Simulation from the SedListOfSimulations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Simulation to get.
   *
   * @return Simulation in this SedListOfSimulations
   * with the given id or NULL if no such
   * Simulation exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual SedSimulation* get(const std::string& sid);


  /**
   * Get a Simulation from the SedListOfSimulations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Simulation to get.
   *
   * @return Simulation in this SedListOfSimulations
   * with the given id or NULL if no such
   * Simulation exists.
   *
   * @see get(unsigned int n)  *
   * @see size()
   */
  virtual const SedSimulation* get(const std::string& sid) const;


  /**
   * Adds a copy the given "Simulation" to this SedListOfSimulations.
   *
   * @param s; the Simulation object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addSimulation(const SedSimulation* s);


  /**
   * Get the number of Simulation objects in this SedListOfSimulations.
   *
   * @return the number of Simulation objects in this SedListOfSimulations
   */
  unsigned int getNumSimulations() const;


  /**
   * Creates a new Simulation object, adds it to the
   * SedListOfSimulations and returns the Simulation object created.
   *
   * @return a new Simulation object instance
   *
   * @see addSimulation(const SedSimulation* s)
   */
  SedUniformTimeCourse* createUniformTimeCourse();


  /**
   * Creates a new Simulation object, adds it to the
   * SedListOfSimulations and returns the Simulation object created.
   *
   * @return a new Simulation object instance
   *
   * @see addSimulation(const SedSimulation* s)
   */
  SedOneStep* createOneStep();


  /**
   * Creates a new Simulation object, adds it to the
   * SedListOfSimulations and returns the Simulation object created.
   *
   * @return a new Simulation object instance
   *
   * @see addSimulation(const SedSimulation* s)
   */
  SedSteadyState* createSteadyState();


  /**
   * Removes the nth Simulation from this SedListOfSimulations
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Simulation to remove.
   *
   * @see size()
   */
  virtual SedSimulation* remove(unsigned int n);


  /**
   * Removes the Simulation from this SedListOfSimulations with the given identifier
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Simulation to remove.
   *
   * @return the Simulation removed. As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedSimulation* remove(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedListOfSimulations, is
   * always @c "sedListOfSimulations".
   *
   * @return the name of this element, i.e. @c "sedListOfSimulations".
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
   * Creates a new Simulation in this SedListOfSimulations
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
SedSimulation_t *
SedSimulation_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedSimulation_free(SedSimulation_t * ss);


LIBSEDML_EXTERN
SedSimulation_t *
SedSimulation_clone(SedSimulation_t * ss);


LIBSEDML_EXTERN
char *
SedSimulation_getId(SedSimulation_t * ss);


LIBSEDML_EXTERN
char *
SedSimulation_getName(SedSimulation_t * ss);


LIBSEDML_EXTERN
SedAlgorithm_t*
SedSimulation_getAlgorithm(SedSimulation_t * ss);


LIBSEDML_EXTERN
SedAlgorithm_t*
SedSimulation_createAlgorithm(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_isSetId(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_isSetName(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_isSetAlgorithm(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_setId(SedSimulation_t * ss, const char * id);


LIBSEDML_EXTERN
int
SedSimulation_setName(SedSimulation_t * ss, const char * name);


LIBSEDML_EXTERN
int
SedSimulation_setAlgorithm(SedSimulation_t * ss, SedAlgorithm_t* algorithm);


LIBSEDML_EXTERN
int
SedSimulation_unsetId(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_unsetName(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_unsetAlgorithm(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_hasRequiredAttributes(SedSimulation_t * ss);


LIBSEDML_EXTERN
int
SedSimulation_hasRequiredElements(SedSimulation_t * ss);


LIBSEDML_EXTERN
SedSimulation_t *
SedListOfSimulations_getById(SedListOf_t * lo, const char * sid);


LIBSEDML_EXTERN
SedSimulation_t *
SedListOfSimulations_removeById(SedListOf_t * lo, const char * sid);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedSimulation_H__  */

