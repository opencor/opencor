/**
 * @file:   SedPlot3D.h
 * @brief:  Implementation of the SedPlot3D class
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


#ifndef SedPlot3D_H__
#define SedPlot3D_H__


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>


#ifdef __cplusplus


#include <string>


#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>
#include <sedml/SedNamespaces.h>
#include <sedml/SedOutput.h>
#include <sedml/SedSurface.h>




LIBSEDML_CPP_NAMESPACE_BEGIN


class LIBSEDML_EXTERN SedPlot3D : public SedOutput
{

protected:

  SedListOfSurfaces   mSurfaces;


public:

  /**
   * Creates a new SedPlot3D with the given level, version, and package version.
   *
   * @param level an unsigned int, the SEDML Level to assign to this SedPlot3D
   *
   * @param version an unsigned int, the SEDML Version to assign to this SedPlot3D
   *
   * @param pkgVersion an unsigned int, the SEDML Sed Version to assign to this SedPlot3D
   */
  SedPlot3D(unsigned int level      = SEDML_DEFAULT_LEVEL,
            unsigned int version    = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedPlot3D with the given SedNamespaces object.
   *
   * @param sedns the SedNamespaces object
   */
  SedPlot3D(SedNamespaces* sedns);


  /**
   * Copy constructor for SedPlot3D.
   *
   * @param orig; the SedPlot3D instance to copy.
   */
  SedPlot3D(const SedPlot3D& orig);


  /**
   * Assignment operator for SedPlot3D.
   *
   * @param rhs; the object whose values are used as the basis
   * of the assignment
   */
  SedPlot3D& operator=(const SedPlot3D& rhs);


  /**
   * Creates and returns a deep copy of this SedPlot3D object.
   *
   * @return a (deep) copy of this SedPlot3D object.
   */
  virtual SedPlot3D* clone() const;


  /**
   * Destructor for SedPlot3D.
   */
  virtual ~SedPlot3D();


  /**
   * Returns the  "SedListOfSurfaces" in this SedPlot3D object.
   *
   * @return the "SedListOfSurfaces" attribute of this SedPlot3D.
   */
  const SedListOfSurfaces* getListOfSurfaces() const;


  /**
   * Get a Surface from the SedListOfSurfaces.
   *
   * @param n the index number of the Surface to get.
   *
   * @return the nth Surface in the SedListOfSurfaces within this SedPlot3D.
   *
   * @see getNumSurfaces()
   */
  SedSurface* getSurface(unsigned int n);


  /**
   * Get a Surface from the SedListOfSurfaces.
   *
   * @param n the index number of the Surface to get.
   *
   * @return the nth Surface in the SedListOfSurfaces within this SedPlot3D.
   *
   * @see getNumSurfaces()
   */
  const SedSurface* getSurface(unsigned int n) const;


  /**
   * Get a Surface from the SedListOfSurfaces
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Surface to get.
   *
   * @return the Surface in the SedListOfSurfaces
   * with the given id or NULL if no such
   * Surface exists.
   *
   * @see getSurface(unsigned int n)
   *
   * @see getNumSurfaces()
   */
  SedSurface* getSurface(const std::string& sid);


  /**
   * Get a Surface from the SedListOfSurfaces
   * based on its identifier.
   *
   * @param sid a string representing the identifier
   * of the Surface to get.
   *
   * @return the Surface in the SedListOfSurfaces
   * with the given id or NULL if no such
   * Surface exists.
   *
   * @see getSurface(unsigned int n)
   *
   * @see getNumSurfaces()
   */
  const SedSurface* getSurface(const std::string& sid) const;


  /**
   * Adds a copy the given "SedSurface" to this SedPlot3D.
   *
   * @param ss; the SedSurface object to add
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif The possible values
   * returned by this function are:
   * @li LIBSEDML_OPERATION_SUCCESS
   * @li LIBSEDML_INVALID_ATTRIBUTE_VALUE
   */
  int addSurface(const SedSurface* ss);


  /**
   * Get the number of SedSurface objects in this SedPlot3D.
   *
   * @return the number of SedSurface objects in this SedPlot3D
   */
  unsigned int getNumSurfaces() const;


  /**
   * Creates a new SedSurface object, adds it to this SedPlot3Ds
   * SedListOfSurfaces and returns the SedSurface object created.
   *
   * @return a new SedSurface object instance
   *
   * @see addSurface(const SedSurface* ss)
   */
  SedSurface* createSurface();


  /**
   * Removes the nth Surface from the SedListOfSurfaces within this SedPlot3D.
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the Surface to remove.
   *
   * @see getNumSurfaces()
   */
  SedSurface* removeSurface(unsigned int n);


  /**
   * Removes the Surface with the given identifier from the SedListOfSurfaces within this SedPlot3D
   * and returns a pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then
   * @c NULL is returned.
   *
   * @param sid the identifier of the Surface to remove.
   *
   * @return the Surface removed. As mentioned above, the caller owns the
   * returned item.
   */
  SedSurface* removeSurface(const std::string& sid);


  /**
   * Returns the XML element name of this object, which for SedPlot3D, is
   * always @c "sedPlot3D".
   *
   * @return the name of this element, i.e. @c "sedPlot3D".
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
   * for this SedPlot3D object have been set.
   *
   * @note The required attributes for a SedPlot3D object are:
   *
   * @return a boolean value indicating whether all the required
   * attributes for this object have been defined.
   */
  virtual bool hasRequiredAttributes() const;


  /**
   * Predicate returning @c true if all the required elements
   * for this SedPlot3D object have been set.
   *
   * @note The required elements for a SedPlot3D object are:
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
SedPlot3D_t *
SedPlot3D_create(unsigned int level, unsigned int version);


LIBSEDML_EXTERN
void
SedPlot3D_free(SedPlot3D_t * spd);


LIBSEDML_EXTERN
SedPlot3D_t *
SedPlot3D_clone(SedPlot3D_t * spd);


LIBSEDML_EXTERN
int
SedPlot3D_addSurface(SedPlot3D_t * spd, SedSurface_t * ss);


LIBSEDML_EXTERN
SedSurface_t *
SedPlot3D_createSurface(SedPlot3D_t * spd);


LIBSEDML_EXTERN
SedListOf_t *
SedPlot3D_getSedListOfSurfaces(SedPlot3D_t * spd);


LIBSEDML_EXTERN
SedSurface_t *
SedPlot3D_getSurface(SedPlot3D_t * spd, unsigned int n);


LIBSEDML_EXTERN
SedSurface_t *
SedPlot3D_getSurfaceById(SedPlot3D_t * spd, const char * sid);


LIBSEDML_EXTERN
unsigned int
SedPlot3D_getNumSurfaces(SedPlot3D_t * spd);


LIBSEDML_EXTERN
SedSurface_t *
SedPlot3D_removeSurface(SedPlot3D_t * spd, unsigned int n);


LIBSEDML_EXTERN
SedSurface_t *
SedPlot3D_removeSurfaceById(SedPlot3D_t * spd, const char * sid);


LIBSEDML_EXTERN
int
SedPlot3D_hasRequiredAttributes(SedPlot3D_t * spd);


LIBSEDML_EXTERN
int
SedPlot3D_hasRequiredElements(SedPlot3D_t * spd);




END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /*  !SWIG  */

#endif /*  SedPlot3D_H__  */

