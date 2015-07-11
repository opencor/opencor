/**
 * @file:   SedPlot2D.h
 * @brief:  Implementation of the SedPlot2D class
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


#ifndef SedPlot2D_H__
#define SedPlot2D_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedOutput.h>
#include <sedml/SedCurve.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedPlot2D : public SedOutput
{

protected:

  SedListOfCurves   mCurves;


public:

  /**
   * Creates a new SedPlot2D with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedPlot2D
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedPlot2D
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedPlot2D
   */
  SedPlot2D(unsigned int level      = SEDML_DEFAULT_LEVEL,
            unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedPlot2D with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedPlot2D(SedNamespaces* sedns);


  /**
   * Copy constructor for SedPlot2D.
   *
   * @param orig; the SedPlot2D instance to copy.
   */
  SedPlot2D(const SedPlot2D& orig);


  /**
   * Assignment operator for SedPlot2D.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedPlot2D& operator=(const SedPlot2D& rhs);


  /**
   * Creates and returns a deep copy of this SedPlot2D object.
   *
   * @return a (deep) copy of this SedPlot2D object.
   */
  virtual SedPlot2D* clone() const;


  /**
   * Destructor for SedPlot2D.
   */
  virtual ~SedPlot2D();


  /**
   * Returns the  "SedListOfCurves" in this SedPlot2D object.
   *
   * @return the "SedListOfCurves" attribute of this SedPlot2D.
   */
  const SedListOfCurves* getListOfCurves() const;


  /**
   * Get a Curve from the SedListOfCurves.
   *
   * @param n the index number of the Curve to get.
   *
   * @return the nth Curve in the SedListOfCurves within this SedPlot2D.
   *
   * @see getNumCurves()
   */
  SedCurve* getCurve(unsigned int n);


  /**
   * Get a Curve from the SedListOfCurves.
   *
   * @param n the index number of the Curve to get.
   *
   * @return the nth Curve in the SedListOfCurves within this SedPlot2D.
   *
   * @see getNumCurves()
   */
  const SedCurve* getCurve(unsigned int n) const;


  /**
   * Get a Curve from the SedListOfCurves
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Curve to get.
   *
   * @return the Curve in the SedListOfCurves
   * with the given id or NULL if no such
   * Curve exists.
   *
   * @see getCurve(unsigned int n)
   *
   * @see getNumCurves()
   */
  SedCurve* getCurve(const std::string& sid);


  /**
   * Get a Curve from the SedListOfCurves
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Curve to get.
   *
   * @return the Curve in the SedListOfCurves
   * with the given id or NULL if no such
   * Curve exists.
   *
   * @see getCurve(unsigned int n)
   *
   * @see getNumCurves()
   */
  const SedCurve* getCurve(const std::string& sid) const;


  /**
   * Adds a copy the given "SedCurve" to this SedPlot2D.
   *
   * @param sc; the SedCurve object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addCurve(const SedCurve* sc);


  /**
   * Get the number of SedCurve objects in this SedPlot2D.
   *
   * @return the number of SedCurve objects in this SedPlot2D
   */
  unsigned int getNumCurves() const;


  /**
   * Creates a new SedCurve object, adds it to this SedPlot2Ds
   * SedListOfCurves and returns the SedCurve object created.
   *
   * @return a new SedCurve object instance
   *
   * @see addCurve(const SedCurve* sc)
   */
  SedCurve* createCurve();


  /**
   * Removes the nth Curve from the SedListOfCurves within this SedPlot2D.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Curve to remove.
   *
   * @see getNumCurves()
   */
  SedCurve* removeCurve(unsigned int n);


  /**
   * Removes the Curve with the given identifier from the SedListOfCurves within this SedPlot2D
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Curve to remove.
   *
   * @return the Curve removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedCurve* removeCurve(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedPlot2D, is
   * always @c "sedPlot2D".
   *
   * @return the name of this element, i.e. @c "sedPlot2D".
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
   * for this SedPlot2D object have been set.
   *
   * @note The required attributes for a SedPlot2D object are:
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedPlot2D object have been set.
   *
   * @note The required elements for a SedPlot2D object are:
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
SedPlot2D_t *
SedPlot2D_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedPlot2D_free(SedPlot2D_t * spd);


LIBSEDML_EXTERN
SedPlot2D_t *
SedPlot2D_clone(SedPlot2D_t * spd);


LIBSEDML_EXTERN
int
SedPlot2D_addCurve(SedPlot2D_t * spd, SedCurve_t * sc);


LIBSEDML_EXTERN
SedCurve_t *
SedPlot2D_createCurve(SedPlot2D_t * spd);


LIBSEDML_EXTERN
SedListOf_t *
SedPlot2D_getSedListOfCurves(SedPlot2D_t * spd);


LIBSEDML_EXTERN
SedCurve_t *
SedPlot2D_getCurve(SedPlot2D_t * spd, unsigned int n);


LIBSEDML_EXTERN
SedCurve_t *
SedPlot2D_getCurveById(SedPlot2D_t * spd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedPlot2D_getNumCurves(SedPlot2D_t * spd);


LIBSEDML_EXTERN
SedCurve_t *
SedPlot2D_removeCurve(SedPlot2D_t * spd, unsigned int n);


LIBSEDML_EXTERN
SedCurve_t *
SedPlot2D_removeCurveById(SedPlot2D_t * spd, const char * sid);


LIBSEDML_EXTERN
int
SedPlot2D_hasRequiredAttributes(SedPlot2D_t * spd);


LIBSEDML_EXTERN
int
SedPlot2D_hasRequiredElements(SedPlot2D_t * spd);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedPlot2D_H__  */

