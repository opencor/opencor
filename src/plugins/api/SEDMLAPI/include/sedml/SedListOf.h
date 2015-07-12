/**
 * @file    SedListOf.h
 * @author  Wraps List and inherits from SedBase
 *
 * <!--------------------------------------------------------------------------
 *
 * This file is part of libSEDML.  Please visit http://sed-ml.org for more
 * information about SED-ML. The latest version of libSEDML can be found on
 * github: https://github.com/fbergmann/libSEDML/
 *
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
 *
 * ---------------------------------------------------------------------- -->
 *
 * @class SedListOf
 * @ingroup Core
 * @brief Parent class for the various Sed "SedListOfXYZ" classes.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * Sed defines various SedListOf___ classes that are containers used for
 * organizing the main components of an Sed model.  All are derived from
 * the abstract class SedBase, and inherit the attributes and subelements of
 * SedBase, such as "metaid" as and "annotation".  The SedListOf___ classes do
 * not add any attributes of their own.
 *
 * The SedListOf class in libSed is a utility class that serves as the parent
 * class for implementing the SedListOf__ classes.  It provides methods for
 * working generically with the various Sed lists of objects in a program.
 * LibSed uses this separate list class rather than ordinary
 * @if clike C&#43;&#43; @endif@if java Java@endif@if python Python@endif@~ lists,
 * so that it can provide the methods and features associated with SedBase.
 *
 * @see SedListOfFunctionDefinitions
 * @see SedListOfUnitDefinitions
 * @see SedListOfCompartmentTypes
 * @see SedListOfSpeciesTypes
 * @see SedListOfCompartments
 * @see SedListOfSpecies
 * @see SedListOfParameters
 * @see SedListOfInitialAssignments
 * @see SedListOfRules
 * @see SedListOfConstraints
 * @see SedListOfReactions
 * @see SedListOfEvents
 */


#ifndef SedListOf_h
#define SedListOf_h


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>
#include <sedml/SedTypeCodes.h>


#ifdef __cplusplus


#include <vector>
#include <algorithm>
#include <functional>

#include <sedml/SedBase.h>

LIBSEDML_CPP_NAMESPACE_BEGIN

class SedVisitor;


/** @cond doxygen-libsbml-internal */
/**
 * Used by SedListOf::get() to lookup an SedBase based by its id.
 */
#ifndef SWIG
template<class CNAME>
struct SedIdEq : public std::unary_function<SedBase*, bool>
{
  const std::string& id;

  SedIdEq(const std::string& id) : id(id) { }
  bool operator()(SedBase* sb)
  { return static_cast <CNAME*>(sb)->getId() == id; }
};
#endif /* SWIG */
/** @endcond */


class LIBSEDML_EXTERN SedListOf : public SedBase
{
public:

  /**
   * Creates a new SedListOf object.
   *
   * @param level the Sed Level; if not assigned, defaults to the
   * value of SEDML_DEFAULT_LEVEL.
   *
   * @param version the Version within the Sed Level; if not assigned,
   * defaults to the value of SEDML_DEFAULT_VERSION.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  SedListOf(unsigned int level   = SEDML_DEFAULT_LEVEL,
            unsigned int version = SEDML_DEFAULT_VERSION);


  /**
   * Creates a new SedListOf with SedNamespaces object.
   *
   * @param sbmlns the set of namespaces that this SedListOf should contain.
   */
  SedListOf(SedNamespaces* sbmlns);


  /**
   * Destroys the given SedListOf and the items inside it.
   */
  virtual ~SedListOf();


  /**
   * Copy constructor;  creates a copy of this SedListOf.
   *
   * @param orig the SedListOf instance to copy.
   */
  SedListOf(const SedListOf& orig);


  /**
   * Assignment operator for SedListOf.
   */
  SedListOf& operator=(const SedListOf& rhs);


  /**
   * Accepts the given SedVisitor.
   *
   * @param v the SedVisitor instance to be used.
   *
   * @return the result of calling <code>v.visit()</code>, which indicates
   * whether the Visitor would like to visit the next item in the
   * list.
   */
  virtual bool accept(SedVisitor& v) const;


  /**
   * Creates and returns a deep copy of this SedListOf.
   *
   * @return a (deep) copy of this SedListOf.
   */
  virtual SedListOf* clone() const;


  /**
   * Adds item to the end of this SedListOf.
   *
   * This variant of the method makes a clone of the @p item handed to it.
   * This means that when the SedListOf is destroyed, the original items will
   * not be destroyed.
   *
   * @param item the item to be added to the list.
   *
   * @see appendAndOwn(SedBase* item)
   */
  int append(const SedBase* item);


  /**
   * Adds item to the end of this SedListOf.
   *
   * This variant of the method does not clone the @p item handed to it;
   * instead, it assumes ownership of it.  This means that when the SedListOf
   * is destroyed, the item will be destroyed along with it.
   *
   * @param item the item to be added to the list.
   *
   * @see append(const SedBase* item)
   */
  int appendAndOwn(SedBase* item);


  /**
   * Adds a clone of all items in the provided SedListOf to this object.  This means that when this SedListOf is destroyed, the original items will not be destroyed.
   *
   * @param list A list of items to be added.
   *
   * @see append(const SedBase* item)
   */
  int appendFrom(const SedListOf* list);


  /**
   * Inserts the item at the given position of this SedListOf
   *
   * This variant of the method makes a clone of the @p item handet to it.
   * This means that when the SedListOf is destroyed, the original items will
   * not be destroyed.
   *
   * @param location the location where to insert the item
   * @param item the item to be inserted to the list
   *
   * @see insertAndOwn(int location, SedBase* item)
   */
  int insert(int location, const SedBase* item);


  /**
   * Inserts the item at the given position of this SedListOf
   *
   * This variant of the method makes a clone of the @p item handet to it.
   * This means that when the SedListOf is destroyed, the original items will
   * not be destroyed.
   *
   * @param location the location where to insert the item
   * @param item the item to be inserted to the list
   *
   * @see insert(int location, const SedBase* item)
   */
  int insertAndOwn(int location, SedBase* item);


  /**
   * Get an item from the list.
   *
   * @param n the index number of the item to get.
   *
   * @return the nth item in this SedListOf items.
   *
   * @see size()
   */
  virtual const SedBase* get(unsigned int n) const;


  /**
   * Get an item from the list.
   *
   * @param n the index number of the item to get.
   *
   * @return the nth item in this SedListOf items.
   *
   * @see size()
   */
  virtual SedBase* get(unsigned int n);


  /**
   * Returns the first child element found that has the given @p id in the model-wide SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the id of objects to find
   *
   * @return pointer to the first element found with the given @p id.
   */
  virtual SedBase* getElementBySId(std::string id);


  /**
   * Returns the first child element it can find with the given @p metaid, or @c NULL if no such object is found.
   *
   * @param metaid string representing the metaid of objects to find
   *
   * @return pointer to the first element found with the given @p metaid.
   */
  virtual SedBase* getElementByMetaId(std::string metaid);

  /**
   * Returns a List of all child SedBase objects, including those nested to an arbitrary depth
   *
   * @return a List of pointers to all children objects.
   */
  virtual List* getAllElements();


#if 0
  /**
   * Get an item from the list based on its identifier.
   *
   * @param sid a string representing the the identifier of the item to get.
   *
   * @return item in this SedListOf items with the given @p sid or @c NULL if no such
   * item exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual const SedBase* get(const std::string& sid) const;
#endif


#if 0
  /**
   * Get an item from the list based on its identifier.
   *
   * @param sid a string representing the the identifier of the item to get.
   *
   * @return item in this SedListOf items with the given @p sid or @c NULL if no such
   * item exists.
   *
   * @see get(unsigned int n)
   * @see size()
   */
  virtual SedBase* get(const std::string& sid);
#endif


  /**
   * Removes all items in this SedListOf object.
   *
   * If parameter @p doDelete is @c true (default), all items in this SedListOf
   * object are deleted and cleared, and thus the caller doesn't have to
   * delete those items.  Otherwise, all items are just cleared from this
   * SedListOf object and the caller is responsible for deleting all items.  (In
   * that case, pointers to all items should be stored elsewhere before
   * calling this function.)
   *
   * @param doDelete if @c true (default), all items are deleted and cleared.
   * Otherwise, all items are just cleared and not deleted.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void clear(bool doDelete = true);


  /**
   * Because SedListOf objects typically live as object children of their parent object and not as pointer children, this function clears itself, but does not attempt to do anything else.  If a particular SedListOf subclass does indeed exist as a pointer only, this function will need to be overridden.
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif@~ The possible values
   * returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   */
  virtual int removeFromParentAndDelete();

  /**
   * Removes the <em>n</em>th item from this SedListOf items and returns a
   * pointer to it.
   *
   * The caller owns the returned item and is responsible for deleting it.
   *
   * @param n the index of the item to remove
   *
   * @see size()
   */
  virtual SedBase* remove(unsigned int n);


#if 0
  /**
   * Removes item in this SedListOf items with the given identifier.
   *
   * The caller owns the returned item and is responsible for deleting it.
   * If none of the items in this list have the identifier @p sid, then @c
   * NULL is returned.
   *
   * @param sid the identifier of the item to remove
   *
   * @return the item removed.  As mentioned above, the caller owns the
   * returned item.
   */
  virtual SedBase* remove(const std::string& sid);
#endif


  /**
   * Get the size of this SedListOf.
   *
   * @return the number of items in this SedListOf items.
   */
  unsigned int size() const;

  /** @cond doxygen-libsbml-internal */

  /**
   * Sets the parent SedDocument of this Sed object.
   *
   * @param d the SedDocument that should become the parent of this
   * SedListOf.
   */
  virtual void setSedDocument(SedDocument* d);


  /**
   * Sets this Sed object to child Sed objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor and assignment operator.
   *
   * @see setSedDocument
   * @see enablePackageInternal
   */
  virtual void connectToChild();


  /** @endcond */

  /**
   * Returns the libSed type code for this object, namely, @c
   * SEDML_LIST_OF.
   *
   * @if clike LibSed attaches an identifying code to every kind of Sed
   * object.  These are known as <em>Sed type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SedTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SEDML_. @endif@if java LibSed attaches an identifying code to every
   * kind of Sed object.  These are known as <em>Sed type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSed, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if python LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the Python language interface for libSed, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if csharp LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the C# language interface for libSed, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SEDML_. @endif@~
   *
   * @return the Sed type code for this object, or @link SedTypeCode_t#Sed_UNKNOWN SEDML_UNKNOWN@endlink (default).
   *
   * @see getElementName()
   */
  virtual int getTypeCode() const;


  /**
   * Get the type code of the objects contained in this SedListOf.
   *
   * @if clike LibSed attaches an identifying code to every kind of Sed
   * object.  These are known as <em>Sed type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SedTypeCode_t.
   * The names of the type codes all begin with the characters @c
   * SEDML_. @endif@if java LibSed attaches an identifying code to every
   * kind of Sed object.  These are known as <em>Sed type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSed, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if python LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the Python language interface for libSed, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @c SEDML_. @endif@if csharp LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the C# language interface for libSed, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml@endlink.  The names of the type codes all begin with
   * the characters @c SEDML_. @endif@~
   *
   * @return the Sed type code for the objects contained in this SedListOf
   * instance, or @link SedTypeCode_t#Sed_UNKNOWN SEDML_UNKNOWN@endlink (default).
   */
  virtual int getItemTypeCode() const;


  /**
   * Returns the XML element name of this object, which for SedListOf, is
   * always @c "listOf".
   *
   * @return the XML name of this element.
   */
  virtual const std::string& getElementName() const;


  /** @cond doxygen-libsbml-internal */
  /**
   * Subclasses should override this method to write out their contained
   * Sed objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.
   */
  virtual void writeElements(XMLOutputStream& stream) const;
  /** @endcond */


protected:
  /** @cond doxygen-libsbml-internal */

  typedef std::vector<SedBase*>           ListItem;
  typedef std::vector<SedBase*>::iterator ListItemIter;

  /**
   * Subclasses should override this method to get the list of
   * expected attributes.
   * This function is invoked from corresponding readAttributes()
   * function.
   */
  virtual void addExpectedAttributes(ExpectedAttributes& attributes);


  /**
   * Subclasses should override this method to read values from the given
   * XMLAttributes set into their specific fields.  Be sure to call your
   * parents implementation of this method as well.
   */
  virtual void readAttributes(const XMLAttributes& attributes,
                              const ExpectedAttributes& expectedAttributes);

  /**
   * Subclasses should override this method to write their XML attributes
   * to the XMLOutputStream.  Be sure to call your parents implementation
   * of this method as well.  For example:
   *
   *   SedBase::writeAttributes(stream);
   *   stream.writeAttribute( "id"  , mId   );
   *   stream.writeAttribute( "name", mName );
   *   ...
   */
  virtual void writeAttributes(XMLOutputStream& stream) const;

  virtual bool isValidTypeForList(SedBase * item) const;

  ListItem mItems;

  /** @endcond */
};

LIBSEDML_CPP_NAMESPACE_END

#endif  /* __cplusplus */


#ifndef SWIG

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


/**
 * Creates a new SedListOf.
 *
 * @return a pointer to created SedListOf.
 */
LIBSEDML_EXTERN
SedListOf_t *
SedListOf_create(unsigned int level, unsigned int version);

/**
 * Frees the given SedListOf and its constituent items.
 *
 * This function assumes each item in the list is derived from SedBase.
 */
LIBSEDML_EXTERN
void
SedListOf_free(SedListOf_t *lo);

/**
 * @return a (deep) copy of this SedListOf items.
 */
LIBSEDML_EXTERN
SedListOf_t *
SedListOf_clone(const SedListOf_t *lo);


/**
 * Adds a copy of item to the end of this SedListOf items.
 */
LIBSEDML_EXTERN
int
SedListOf_append(SedListOf_t *lo, const SedBase_t *item);

/**
 * Adds the given item to the end of this SedListOf items.
 */
LIBSEDML_EXTERN
int
SedListOf_appendAndOwn(SedListOf_t *lo, SedBase_t *item);

/**
 * Adds clones of the given items from the second list to the end of this SedListOf items.
 */
LIBSEDML_EXTERN
int
SedListOf_appendFrom(SedListOf_t *lo, SedListOf_t *list);

/**
 * inserts a copy of item to this SedListOf items at the given position.
 */
LIBSEDML_EXTERN
int
SedListOf_insert(SedListOf_t *lo, int location, const SedBase_t *item);

/**
 * inserts the item to this SedListOf items at the given position.
 */
LIBSEDML_EXTERN
int
SedListOf_insertAndOwn(SedListOf_t *lo, int location, SedBase_t *item);


/**
 * Returns the nth item in this SedListOf items.
 */
LIBSEDML_EXTERN
SedBase_t *
SedListOf_get(SedListOf_t *lo, unsigned int n);

#if (0)
/**
 * @return item in this SedListOf items with the given @p sid or @c NULL if no such
 * item exists.
 */
LIBSEDML_EXTERN
SedBase_t *
SedListOf_getById(SedListOf_t *lo, const char *sid);
#endif

/**
 * Removes all items in this SedListOf object.
 */
LIBSEDML_EXTERN
void
SedListOf_clear(SedListOf_t *lo, int doDelete);

/**
 * Removes the nth item from this SedListOf items and returns a pointer to
 * it.  The caller owns the returned item and is responsible for deleting
 * it.
 */
LIBSEDML_EXTERN
SedBase_t *
SedListOf_remove(SedListOf_t *lo, unsigned int n);

#if (0)
/**
 * Removes item in this SedListOf items with the given @p sid or @c NULL if no such
 * item exists.  The caller owns the returned item and is repsonsible for
 * deleting it.
 */
LIBSEDML_EXTERN
SedBase_t *
SedListOf_removeById(SedListOf_t *lo, const char *sid);
#endif

/**
 * Returns the number of items in this SedListOf items.
 */
LIBSEDML_EXTERN
unsigned int
SedListOf_size(const SedListOf_t *lo);

/**
 * @return the int of Sed objects contained in this SedListOf or
 * SEDML_UNKNOWN (default).
 */
LIBSEDML_EXTERN
int
SedListOf_getItemTypeCode(const SedListOf_t *lo);


END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /* !SWIG */
#endif  /* SedListOf_h */

