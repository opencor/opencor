/**
 * @file:   SedDocument.h
 * @brief:  Implementation of the SedDocument class
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


#ifndef SedDocument_H__
#define SedDocument_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedSimulation.h>
#include <sedml/SedModel.h>
#include <sedml/SedTask.h>
#include <sedml/SedDataGenerator.h>
#include <sedml/SedOutput.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedDocument : public SedBase
{

protected:

  int           mLevel;
  bool          mIsSetLevel;
  int           mVersion;
  bool          mIsSetVersion;
  SedListOfSimulations   mSimulations;
  SedListOfModels   mModels;
  SedListOfTasks   mTasks;
  SedListOfDataGenerators   mDataGenerators;
  SedListOfOutputs   mOutputs;


public:

  /**
   * Creates a new SedDocument with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedDocument
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedDocument
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedDocument
   */
  SedDocument(unsigned int level      = SEDML_DEFAULT_LEVEL,
              unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedDocument with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedDocument(SedNamespaces* sedns);


  /**
   * Copy constructor for SedDocument.
   *
   * @param orig; the SedDocument instance to copy.
   */
  SedDocument(const SedDocument& orig);


  /**
   * Assignment operator for SedDocument.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedDocument& operator=(const SedDocument& rhs);


  /**
   * Creates and returns a deep copy of this SedDocument object.
   *
   * @return a (deep) copy of this SedDocument object.
   */
  virtual SedDocument* clone() const;


  /**
   * Destructor for SedDocument.
   */
  virtual ~SedDocument();


  /**
   * Returns the value of the "level" attribute of this SedDocument.
   *
   * @return the value of the "level" attribute of this SedDocument as a integer.
   */
  virtual const int getLevel() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDocument's "level" attribute has been set.
   *
   * @return @c true if this SedDocument's "level" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetLevel() const;


  /**
   * Sets the value of the "level" attribute of this SedDocument.
   *
   * @param level; int value of the "level" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setLevel(int level);


  /**
   * Unsets the value of the "level" attribute of this SedDocument.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetLevel();


  /**
   * Returns the value of the "version" attribute of this SedDocument.
   *
   * @return the value of the "version" attribute of this SedDocument as a integer.
   */
  virtual const int getVersion() const;


  /**
   * Predicate returning @c true or @c false depending on whether this
   * SedDocument's "version" attribute has been set.
   *
   * @return @c true if this SedDocument's "version" attribute has been set,
   * otherwise @c false is returned.
   */
  virtual bool isSetVersion() const;


  /**
   * Sets the value of the "version" attribute of this SedDocument.
   *
   * @param version; int value of the "version" attribute to be set
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  virtual int setVersion(int version);


  /**
   * Unsets the value of the "version" attribute of this SedDocument.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_OPERATION_FAILED
   */
  virtual int unsetVersion();


  /**
   * Returns the  "SedListOfSimulations" in this SedDocument object.
   *
   * @return the "SedListOfSimulations" attribute of this SedDocument.
   */
  const SedListOfSimulations* getListOfSimulations() const;


  /**
   * Get a Simulation from the SedListOfSimulations.
   *
   * @param n the index number of the Simulation to get.
   *
   * @return the nth Simulation in the SedListOfSimulations within this SedDocument.
   *
   * @see getNumSimulations()
   */
  SedSimulation* getSimulation(unsigned int n);


  /**
   * Get a Simulation from the SedListOfSimulations.
   *
   * @param n the index number of the Simulation to get.
   *
   * @return the nth Simulation in the SedListOfSimulations within this SedDocument.
   *
   * @see getNumSimulations()
   */
  const SedSimulation* getSimulation(unsigned int n) const;


  /**
   * Get a Simulation from the SedListOfSimulations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Simulation to get.
   *
   * @return the Simulation in the SedListOfSimulations
   * with the given id or NULL if no such
   * Simulation exists.
   *
   * @see getSimulation(unsigned int n)
   *
   * @see getNumSimulations()
   */
  SedSimulation* getSimulation(const std::string& sid);


  /**
   * Get a Simulation from the SedListOfSimulations
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Simulation to get.
   *
   * @return the Simulation in the SedListOfSimulations
   * with the given id or NULL if no such
   * Simulation exists.
   *
   * @see getSimulation(unsigned int n)
   *
   * @see getNumSimulations()
   */
  const SedSimulation* getSimulation(const std::string& sid) const;


  /**
   * Adds a copy the given "SedSimulation" to this SedDocument.
   *
   * @param ss; the SedSimulation object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addSimulation(const SedSimulation* ss);


  /**
   * Get the number of SedSimulation objects in this SedDocument.
   *
   * @return the number of SedSimulation objects in this SedDocument
   */
  unsigned int getNumSimulations() const;


  /**
   * Creates a new SedUniformTimeCourse object, adds it to this SedDocuments
   * SedListOfSimulations and returns the SedUniformTimeCourse object created.
   *
   * @return a new SedUniformTimeCourse object instance
   *
   * @see addSedSimulation(const SedSimulation* ss)
   */
  SedUniformTimeCourse* createUniformTimeCourse();


  /**
   * Creates a new SedOneStep object, adds it to this SedDocuments
   * SedListOfSimulations and returns the SedOneStep object created.
   *
   * @return a new SedOneStep object instance
   *
   * @see addSedSimulation(const SedSimulation* ss)
   */
  SedOneStep* createOneStep();


  /**
   * Creates a new SedSteadyState object, adds it to this SedDocuments
   * SedListOfSimulations and returns the SedSteadyState object created.
   *
   * @return a new SedSteadyState object instance
   *
   * @see addSedSimulation(const SedSimulation* ss)
   */
  SedSteadyState* createSteadyState();


  /**
   * Removes the nth Simulation from the SedListOfSimulations within this SedDocument.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Simulation to remove.
   *
   * @see getNumSimulations()
   */
  SedSimulation* removeSimulation(unsigned int n);


  /**
   * Removes the Simulation with the given identifier from the SedListOfSimulations within this SedDocument
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
  SedSimulation* removeSimulation(const std::string& sid);


  /**
   * Returns the  "SedListOfModels" in this SedDocument object.
   *
   * @return the "SedListOfModels" attribute of this SedDocument.
   */
  const SedListOfModels* getListOfModels() const;


  /**
   * Get a Model from the SedListOfModels.
   *
   * @param n the index number of the Model to get.
   *
   * @return the nth Model in the SedListOfModels within this SedDocument.
   *
   * @see getNumModels()
   */
  SedModel* getModel(unsigned int n);


  /**
   * Get a Model from the SedListOfModels.
   *
   * @param n the index number of the Model to get.
   *
   * @return the nth Model in the SedListOfModels within this SedDocument.
   *
   * @see getNumModels()
   */
  const SedModel* getModel(unsigned int n) const;


  /**
   * Get a Model from the SedListOfModels
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Model to get.
   *
   * @return the Model in the SedListOfModels
   * with the given id or NULL if no such
   * Model exists.
   *
   * @see getModel(unsigned int n)
   *
   * @see getNumModels()
   */
  SedModel* getModel(const std::string& sid);


  /**
   * Get a Model from the SedListOfModels
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Model to get.
   *
   * @return the Model in the SedListOfModels
   * with the given id or NULL if no such
   * Model exists.
   *
   * @see getModel(unsigned int n)
   *
   * @see getNumModels()
   */
  const SedModel* getModel(const std::string& sid) const;


  /**
   * Adds a copy the given "SedModel" to this SedDocument.
   *
   * @param sm; the SedModel object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addModel(const SedModel* sm);


  /**
   * Get the number of SedModel objects in this SedDocument.
   *
   * @return the number of SedModel objects in this SedDocument
   */
  unsigned int getNumModels() const;


  /**
   * Creates a new SedModel object, adds it to this SedDocuments
   * SedListOfModels and returns the SedModel object created.
   *
   * @return a new SedModel object instance
   *
   * @see addModel(const SedModel* sm)
   */
  SedModel* createModel();


  /**
   * Removes the nth Model from the SedListOfModels within this SedDocument.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Model to remove.
   *
   * @see getNumModels()
   */
  SedModel* removeModel(unsigned int n);


  /**
   * Removes the Model with the given identifier from the SedListOfModels within this SedDocument
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
  SedModel* removeModel(const std::string& sid);


  /**
   * Returns the  "SedListOfTasks" in this SedDocument object.
   *
   * @return the "SedListOfTasks" attribute of this SedDocument.
   */
  const SedListOfTasks* getListOfTasks() const;


  /**
   * Get a Task from the SedListOfTasks.
   *
   * @param n the index number of the Task to get.
   *
   * @return the nth Task in the SedListOfTasks within this SedDocument.
   *
   * @see getNumTasks()
   */
  SedTask* getTask(unsigned int n);


  /**
   * Get a Task from the SedListOfTasks.
   *
   * @param n the index number of the Task to get.
   *
   * @return the nth Task in the SedListOfTasks within this SedDocument.
   *
   * @see getNumTasks()
   */
  const SedTask* getTask(unsigned int n) const;


  /**
   * Get a Task from the SedListOfTasks
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Task to get.
   *
   * @return the Task in the SedListOfTasks
   * with the given id or NULL if no such
   * Task exists.
   *
   * @see getTask(unsigned int n)
   *
   * @see getNumTasks()
   */
  SedTask* getTask(const std::string& sid);


  /**
   * Get a Task from the SedListOfTasks
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Task to get.
   *
   * @return the Task in the SedListOfTasks
   * with the given id or NULL if no such
   * Task exists.
   *
   * @see getTask(unsigned int n)
   *
   * @see getNumTasks()
   */
  const SedTask* getTask(const std::string& sid) const;


  /**
   * Adds a copy the given "SedTask" to this SedDocument.
   *
   * @param st; the SedTask object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addTask(const SedTask* st);


  /**
   * Get the number of SedTask objects in this SedDocument.
   *
   * @return the number of SedTask objects in this SedDocument
   */
  unsigned int getNumTasks() const;


  /**
   * Creates a new SedTask object, adds it to this SedDocuments
   * SedListOfTasks and returns the SedTask object created.
   *
   * @return a new SedTask object instance
   *
   * @see addSedTask(const SedTask* st)
   */
  SedTask* createTask();


  /**
   * Creates a new SedRepeatedTask object, adds it to this SedDocuments
   * SedListOfTasks and returns the SedRepeatedTask object created.
   *
   * @return a new SedRepeatedTask object instance
   *
   * @see addSedTask(const SedTask* st)
   */
  SedRepeatedTask* createRepeatedTask();


  /**
   * Removes the nth Task from the SedListOfTasks within this SedDocument.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Task to remove.
   *
   * @see getNumTasks()
   */
  SedTask* removeTask(unsigned int n);


  /**
   * Removes the Task with the given identifier from the SedListOfTasks within this SedDocument
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Task to remove.
   *
   * @return the Task removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedTask* removeTask(const std::string& sid);


  /**
   * Returns the  "SedListOfDataGenerators" in this SedDocument object.
   *
   * @return the "SedListOfDataGenerators" attribute of this SedDocument.
   */
  const SedListOfDataGenerators* getListOfDataGenerators() const;


  /**
   * Get a DataGenerator from the SedListOfDataGenerators.
   *
   * @param n the index number of the DataGenerator to get.
   *
   * @return the nth DataGenerator in the SedListOfDataGenerators within this SedDocument.
   *
   * @see getNumDataGenerators()
   */
  SedDataGenerator* getDataGenerator(unsigned int n);


  /**
   * Get a DataGenerator from the SedListOfDataGenerators.
   *
   * @param n the index number of the DataGenerator to get.
   *
   * @return the nth DataGenerator in the SedListOfDataGenerators within this SedDocument.
   *
   * @see getNumDataGenerators()
   */
  const SedDataGenerator* getDataGenerator(unsigned int n) const;


  /**
   * Get a DataGenerator from the SedListOfDataGenerators
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataGenerator to get.
   *
   * @return the DataGenerator in the SedListOfDataGenerators
   * with the given id or NULL if no such
   * DataGenerator exists.
   *
   * @see getDataGenerator(unsigned int n)
   *
   * @see getNumDataGenerators()
   */
  SedDataGenerator* getDataGenerator(const std::string& sid);


  /**
   * Get a DataGenerator from the SedListOfDataGenerators
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the DataGenerator to get.
   *
   * @return the DataGenerator in the SedListOfDataGenerators
   * with the given id or NULL if no such
   * DataGenerator exists.
   *
   * @see getDataGenerator(unsigned int n)
   *
   * @see getNumDataGenerators()
   */
  const SedDataGenerator* getDataGenerator(const std::string& sid) const;


  /**
   * Adds a copy the given "SedDataGenerator" to this SedDocument.
   *
   * @param sdg; the SedDataGenerator object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addDataGenerator(const SedDataGenerator* sdg);


  /**
   * Get the number of SedDataGenerator objects in this SedDocument.
   *
   * @return the number of SedDataGenerator objects in this SedDocument
   */
  unsigned int getNumDataGenerators() const;


  /**
   * Creates a new SedDataGenerator object, adds it to this SedDocuments
   * SedListOfDataGenerators and returns the SedDataGenerator object created.
   *
   * @return a new SedDataGenerator object instance
   *
   * @see addDataGenerator(const SedDataGenerator* sdg)
   */
  SedDataGenerator* createDataGenerator();


  /**
   * Removes the nth DataGenerator from the SedListOfDataGenerators within this SedDocument.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the DataGenerator to remove.
   *
   * @see getNumDataGenerators()
   */
  SedDataGenerator* removeDataGenerator(unsigned int n);


  /**
   * Removes the DataGenerator with the given identifier from the SedListOfDataGenerators within this SedDocument
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
  SedDataGenerator* removeDataGenerator(const std::string& sid);


  /**
   * Returns the  "SedListOfOutputs" in this SedDocument object.
   *
   * @return the "SedListOfOutputs" attribute of this SedDocument.
   */
  const SedListOfOutputs* getListOfOutputs() const;


  /**
   * Get a Output from the SedListOfOutputs.
   *
   * @param n the index number of the Output to get.
   *
   * @return the nth Output in the SedListOfOutputs within this SedDocument.
   *
   * @see getNumOutputs()
   */
  SedOutput* getOutput(unsigned int n);


  /**
   * Get a Output from the SedListOfOutputs.
   *
   * @param n the index number of the Output to get.
   *
   * @return the nth Output in the SedListOfOutputs within this SedDocument.
   *
   * @see getNumOutputs()
   */
  const SedOutput* getOutput(unsigned int n) const;


  /**
   * Get a Output from the SedListOfOutputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Output to get.
   *
   * @return the Output in the SedListOfOutputs
   * with the given id or NULL if no such
   * Output exists.
   *
   * @see getOutput(unsigned int n)
   *
   * @see getNumOutputs()
   */
  SedOutput* getOutput(const std::string& sid);


  /**
   * Get a Output from the SedListOfOutputs
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Output to get.
   *
   * @return the Output in the SedListOfOutputs
   * with the given id or NULL if no such
   * Output exists.
   *
   * @see getOutput(unsigned int n)
   *
   * @see getNumOutputs()
   */
  const SedOutput* getOutput(const std::string& sid) const;


  /**
   * Adds a copy the given "SedOutput" to this SedDocument.
   *
   * @param so; the SedOutput object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addOutput(const SedOutput* so);


  /**
   * Get the number of SedOutput objects in this SedDocument.
   *
   * @return the number of SedOutput objects in this SedDocument
   */
  unsigned int getNumOutputs() const;


  /**
   * Creates a new SedReport object, adds it to this SedDocuments
   * SedListOfOutputs and returns the SedReport object created.
   *
   * @return a new SedReport object instance
   *
   * @see addSedOutput(const SedOutput* so)
   */
  SedReport* createReport();


  /**
   * Creates a new SedPlot2D object, adds it to this SedDocuments
   * SedListOfOutputs and returns the SedPlot2D object created.
   *
   * @return a new SedPlot2D object instance
   *
   * @see addSedOutput(const SedOutput* so)
   */
  SedPlot2D* createPlot2D();


  /**
   * Creates a new SedPlot3D object, adds it to this SedDocuments
   * SedListOfOutputs and returns the SedPlot3D object created.
   *
   * @return a new SedPlot3D object instance
   *
   * @see addSedOutput(const SedOutput* so)
   */
  SedPlot3D* createPlot3D();


  /**
   * Removes the nth Output from the SedListOfOutputs within this SedDocument.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Output to remove.
   *
   * @see getNumOutputs()
   */
  SedOutput* removeOutput(unsigned int n);


  /**
   * Removes the Output with the given identifier from the SedListOfOutputs within this SedDocument
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Output to remove.
   *
   * @return the Output removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedOutput* removeOutput(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedDocument, is
   * always @c "sedDocument".
   *
   * @return the name of this element, i.e. @c "sedDocument".
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
   * for this SedDocument object have been set.
   *
   * @note The required attributes for a SedDocument object are:
   * @li "level"
   * @li "version"
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedDocument object have been set.
   *
   * @note The required elements for a SedDocument object are:
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



public:

  /**
   * Returns the list of errors or warnings logged during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * @return the SedErrorLog used for this SedDocument
   *
   * @see SedDocument::getNumErrors()
   */
  virtual SedErrorLog* getErrorLog();


  /**
   * Returns a constant pointer to the list of errors or warnings
   * logged during parsing, consistency checking, or attempted translation
   * of this model.
   *
   * @return the SedErrorLog used for this SedDocument
   *
   * @see SedDocument::getNumErrors()
   */
  const SedErrorLog* getErrorLog() const;

  /**
   * Returns the nth error or warning encountered during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * Callers can use method XMLError::getSeverity() on the result to assess
   * the severity of the problem.  The possible severity levels range from
   * informational messages to fatal errors.
   *
   * @return the error or warning indexed by integer @p n, or return @c
   * NULL if <code>n &gt; (getNumErrors() - 1)</code>.
   *
   * @param n the integer index of the error sought.
   *
   * @see SedDocument::getNumErrors()
   */
  const SedError* getError(unsigned int n) const;


  /**
   * Returns the number of errors or warnings encountered during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * @return the number of errors or warnings encountered
   *
   * @see SedDocument::getError(unsigned int n)
   */
  unsigned int getNumErrors() const;

  /**
   * Returns the number of errors or warnings encountered with the given
   * severity during parsing,
   * consistency checking, or attempted translation of this model.
   *
   * @param severity the severity of the error sought.
   *
   * @return the number of errors or warnings encountered
   *
   * @see SedDocument::getError(unsigned int n)
   */
  unsigned int getNumErrors(unsigned int severity) const;

  /**
   * Returns a list of XML Namespaces associated with the XML content
   * of this SED-ML document.
   *
   * @return the XML Namespaces associated with this SED-ML object
   */
  virtual XMLNamespaces* getNamespaces() const;

protected:
  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.
   *
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;
private:

  SedErrorLog mErrorLog;

};



LIBSEDML_CPP_NAMESPACE_END

#endif  /*  __cplusplus  */

#ifndef SWIG

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

LIBSEDML_EXTERN
SedDocument_t *
SedDocument_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedDocument_free(SedDocument_t * sd);


LIBSEDML_EXTERN
SedDocument_t *
SedDocument_clone(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_getLevel(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_getVersion(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_isSetLevel(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_isSetVersion(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_setLevel(SedDocument_t * sd, int level);


LIBSEDML_EXTERN
int
SedDocument_setVersion(SedDocument_t * sd, int version);


LIBSEDML_EXTERN
int
SedDocument_unsetLevel(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_unsetVersion(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_addSimulation(SedDocument_t * sd, SedSimulation_t * ss);


LIBSEDML_EXTERN
SedUniformTimeCourse_t *
SedDocument_createUniformTimeCourse(SedDocument_t * sd);


LIBSEDML_EXTERN
SedOneStep_t *
SedDocument_createOneStep(SedDocument_t * sd);


LIBSEDML_EXTERN
SedSteadyState_t *
SedDocument_createSteadyState(SedDocument_t * sd);


LIBSEDML_EXTERN
SedListOf_t *
SedDocument_getSedListOfSimulations(SedDocument_t * sd);


LIBSEDML_EXTERN
SedSimulation_t *
SedDocument_getSimulation(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedSimulation_t *
SedDocument_getSimulationById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDocument_getNumSimulations(SedDocument_t * sd);


LIBSEDML_EXTERN
SedSimulation_t *
SedDocument_removeSimulation(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedSimulation_t *
SedDocument_removeSimulationById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
int
SedDocument_addModel(SedDocument_t * sd, SedModel_t * sm);


LIBSEDML_EXTERN
SedModel_t *
SedDocument_createModel(SedDocument_t * sd);


LIBSEDML_EXTERN
SedListOf_t *
SedDocument_getSedListOfModels(SedDocument_t * sd);


LIBSEDML_EXTERN
SedModel_t *
SedDocument_getModel(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedModel_t *
SedDocument_getModelById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDocument_getNumModels(SedDocument_t * sd);


LIBSEDML_EXTERN
SedModel_t *
SedDocument_removeModel(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedModel_t *
SedDocument_removeModelById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
int
SedDocument_addTask(SedDocument_t * sd, SedTask_t * st);


LIBSEDML_EXTERN
SedTask_t *
SedDocument_createTask(SedDocument_t * sd);


LIBSEDML_EXTERN
SedRepeatedTask_t *
SedDocument_createRepeatedTask(SedDocument_t * sd);


LIBSEDML_EXTERN
SedListOf_t *
SedDocument_getSedListOfTasks(SedDocument_t * sd);


LIBSEDML_EXTERN
SedTask_t *
SedDocument_getTask(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedTask_t *
SedDocument_getTaskById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDocument_getNumTasks(SedDocument_t * sd);


LIBSEDML_EXTERN
SedTask_t *
SedDocument_removeTask(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedTask_t *
SedDocument_removeTaskById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
int
SedDocument_addDataGenerator(SedDocument_t * sd, SedDataGenerator_t * sdg);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDocument_createDataGenerator(SedDocument_t * sd);


LIBSEDML_EXTERN
SedListOf_t *
SedDocument_getSedListOfDataGenerators(SedDocument_t * sd);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDocument_getDataGenerator(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDocument_getDataGeneratorById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDocument_getNumDataGenerators(SedDocument_t * sd);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDocument_removeDataGenerator(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedDataGenerator_t *
SedDocument_removeDataGeneratorById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
int
SedDocument_addOutput(SedDocument_t * sd, SedOutput_t * so);


LIBSEDML_EXTERN
SedReport_t *
SedDocument_createReport(SedDocument_t * sd);


LIBSEDML_EXTERN
SedPlot2D_t *
SedDocument_createPlot2D(SedDocument_t * sd);


LIBSEDML_EXTERN
SedPlot3D_t *
SedDocument_createPlot3D(SedDocument_t * sd);


LIBSEDML_EXTERN
SedListOf_t *
SedDocument_getSedListOfOutputs(SedDocument_t * sd);


LIBSEDML_EXTERN
SedOutput_t *
SedDocument_getOutput(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedOutput_t *
SedDocument_getOutputById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedDocument_getNumOutputs(SedDocument_t * sd);


LIBSEDML_EXTERN
SedOutput_t *
SedDocument_removeOutput(SedDocument_t * sd, unsigned int n);


LIBSEDML_EXTERN
SedOutput_t *
SedDocument_removeOutputById(SedDocument_t * sd, const char * sid);


LIBSEDML_EXTERN
int
SedDocument_hasRequiredAttributes(SedDocument_t * sd);


LIBSEDML_EXTERN
int
SedDocument_hasRequiredElements(SedDocument_t * sd);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedDocument_H__  */

