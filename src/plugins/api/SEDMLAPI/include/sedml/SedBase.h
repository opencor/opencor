/**
 * @file    SedBase.h
 * @brief   Definition of SedBase, the base object of all Sed objects
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
 * @class SedBase
 * @ingroup Core
 * @brief Implementation of %SedBase, the base class of most Sed objects.
 *
 * Most components in Sed are derived from a single abstract base type,
 * SedBase.  In addition to serving as the parent class for most other
 * classes of objects in Sed, this base type is designed to allow a
 * modeler or a software package to attach arbitrary information to each
 * major element or list in an Sed model.
 *
 * SedBase has an optional subelement called "notes".  It is intended to
 * serve as a place for storing optional information intended to be seen by
 * humans.  An example use of the "notes" element would be to contain
 * formatted user comments about the model element in which the "notes"
 * element is enclosed.  There are certain conditions on the XHTML content
 * permitted inside the "notes" element; please consult the <a
 * target="_blank" href="http://sbml.org/Documents/Specifications">Sed
 * specification document</a> corresponding to the Sed Level and Version
 * of your model for more information about the requirements for "notes"
 * content.
 *
 * SedBase has another optional subelement called "annotation".  Whereas the
 * "notes" element described above is a container for content to be shown
 * directly to humans, the "annotation" element is a container for optional
 * software-generated content @em not meant to be shown to humans.  The
 * element's content type is <a target="_blank"
 * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
 * "any"</a>, allowing essentially arbitrary data content.  Sed places
 * only a few restrictions on the organization of the content; these are
 * intended to help software tools read and write the data as well as help
 * reduce conflicts between annotations added by different tools.  As is
 * the case with "notes", it is important to refer to the <a
 * target="_blank" href="http://sbml.org/Documents/Specifications">Sed
 * specification document</a> corresponding to the Sed Level and Version
 * of your model for more information about the requirements for
 * "annotation" content.
 *
 * It is worth pointing out that the "annotation" element in the definition
 * of SedBase exists in order that software developers may attach optional
 * application-specific data to the elements in an Sed model.  However, it
 * is important that this facility not be misused.  In particular, it is
 * <em>critical</em> that data essential to a model definition or that can
 * be encoded in existing Sed elements is <em>not</em> stored in
 * "annotation". Parameter values, functional dependencies between model
 * elements, etc., should not be recorded as annotations.  It is crucial to
 * keep in mind the fact that data placed in annotations can be freely
 * ignored by software applications.  If such data affects the
 * interpretation of a model, then software interoperability is greatly
 * impeded.
 *
 * Sed Level 2 introduced an optional SedBase attribute named "metaid" for
 * supporting metadata annotations using RDF (<a target="_blank"
 * href="http://www.w3.org/RDF/">Resource Description Format</a>). The
 * attribute value has the data type <a
 * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML identifier
 * type, which means each "metaid" value must be globally unique within an
 * Sed file.  (Importantly, this uniqueness criterion applies across any
 * attribute with type <a href="http://www.w3.org/TR/REC-xml/#id">XML
 * ID</a>, not just the "metaid" attribute used by Sed&mdash;something to
 * be aware of if your application-specific XML content inside the
 * "annotation" subelement happens to use <a
 * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)  The "metaid" value
 * serves to identify a model component for purposes such as referencing
 * that component from metadata placed within "annotation" subelements.
 *
 * Beginning with Sed Level 2 Version 3, SedBase also has an optional
 * attribute named "sboTerm" for supporting the use of the Systems Biology
 * Ontology.  In Sed proper, the data type of the attribute is a string of
 * the form "SBO:NNNNNNN", where "NNNNNNN" is a seven digit integer number;
 * libSed simplifies the representation by only storing the "NNNNNNN"
 * integer portion.  Thus, in libSed, the "sboTerm" attribute on SedBase has
 * data type @c int, and SBO identifiers are stored simply as integers.
 * (For convenience, SedBase offers methods for returning both the integer
 * form and a text-string form of the SBO identifier.)  SBO terms are a
 * type of optional annotation, and each different class of Sed object
 * derived from SedBase imposes its own requirements about the values
 * permitted for "sboTerm".  Please consult the Sed Level&nbsp;2
 * Version&nbsp;4 specification for more information about the use of SBO
 * and the "sboTerm" attribute.
 *
 * Finally, note that, in the list of methods on SedBase, there is no public
 * constructor because SedBase is an abstract class.  The constructors reside
 * in the subclasses derived from SedBase.
 *
 *
 * @section sbase-miriam Standard format for annotations linking data resources
 *
 * Sed Level 2 Versions 2, 3 and 4, and Level&nbsp;3, define a proposed
 * regular format for encoding two particular categories of annotations:
 * (a) references to controlled vocabulary terms and database identifiers
 * which define and describe biological and biochemical entities in a
 * model; and (b) descriptions of the provenance of a model, including its
 * author(s) and modification history.
 */


#ifndef SedBase_h
#define SedBase_h


#include <sedml/common/extern.h>
#include <sedml/common/libsedml-namespace.h>
#include <sedml/common/sedmlfwd.h>
#include <sedml/SedTypeCodes.h>
#include <sedml/SedNamespaces.h>
#include <sbml/util/List.h>
#include <sbml/SyntaxChecker.h>
#include <sedml/SedConstructorException.h>
#include <sbml/ExpectedAttributes.h>
#include <sbml/xml/XMLNode.h>

#ifdef __cplusplus


#include <string>
#include <stdexcept>
#include <algorithm>

#include <sedml/SedErrorLog.h>



LIBSBML_CPP_NAMESPACE_BEGIN

class Model;

class List;

class XMLAttributes;
class XMLInputStream;
class XMLNamespaces;
class XMLOutputStream;
class XMLToken;

LIBSBML_CPP_NAMESPACE_END

LIBSEDML_CPP_NAMESPACE_BEGIN

//class SedErrorLog;
class SedVisitor;
class SedDocument;





class LIBSEDML_EXTERN SedBase
{
public:

  /**
   * Destroys this object.
   */
  virtual ~SedBase();


  /**
   * Assignment operator for SedBase.
   *
   * @param rhs The object whose values are used as the basis of the
   * assignment.
   *
   * @throws @if python ValueError @else SedConstructorException @endif@~
   * Thrown if the argument @p rhs is @c NULL.
   */
  SedBase& operator=(const SedBase& rhs);


  /**
   * Accepts the given SedVisitor for this SedBase object.
   *
   * @param v the SedVisitor instance to be used
   *
   * @return the result of calling <code>v.visit()</code>.
   */
  virtual bool accept(SedVisitor& v) const = 0;


  /**
   * Creates and returns a deep copy of this SedBase object.
   *
   * @return a (deep) copy of this SedBase object.
   */
  virtual SedBase* clone() const = 0;


  /**
   * Returns the first child element found that has the given @p id in the
   * model-wide @c SId namespace, or @c NULL if no such object is found.
   *
   * @param id string representing the "id" attribute value of the object
   * to find.
   *
   * @return pointer to the first element found with the given identifier.
   */
  virtual SedBase* getElementBySId(std::string id);


  /**
   * Returns the first child element it can find with a specific "metaid"
   * attribute value, or @c NULL if no such object is found.
   *
   * @param metaid string representing the "metaid" attribute value of the
   * object to find.
   *
   * @return pointer to the first element found with the given meta-identifier.
   */
  virtual SedBase* getElementByMetaId(std::string metaid);


  /**
   * Returns a List of all child SedBase objects, including those nested to
   * an arbitrary depth.
   *
   * @return a pointer to a List of pointers to all children objects.
   */
  virtual List* getAllElements();


  /**
   * Returns the value of the "metaid" attribute of this object.
   *
   * The optional attribute named "metaid", present on every major Sed
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an Sed file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by Sed&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *
   * @return the meta-identifier of this Sed object.
   *
   * @see isSetMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  const std::string& getMetaId() const;


  /**
   * Returns the value of the "metaid" attribute of this object.
   *
   * The optional attribute named "metaid", present on every major Sed
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format).  The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an Sed file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by Sed&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model
   * component for purposes such as referencing that component from
   * metadata placed within "annotation" subelements.
   *
   * @return the meta-identifier of this Sed object, as a string.
   *
   * @see isSetMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  std::string& getMetaId();


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   *
   * Returns the value of the "id" attribute of this object, if it has one.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  The identifier given by an object's "id" attribute value
   * is used to identify the object within the Sed model definition.
   * Other objects can refer to the component using this identifier.  The
   * data type of "id" is always either <code>Sid</code> or
   * <code>UnitSId</code>, depending on the object in question.  Both
   * data types are defined as follows:
   * <pre style="margin-left: 2em; border: none; font-weight: bold; color: black">
   *   letter ::= 'a'..'z','A'..'Z'
   *   digit  ::= '0'..'9'
   *   idChar ::= letter | digit | '_'
   *   SId    ::= ( letter | '_' ) idChar*
   * </pre>
   *
   * The equality of <code>SId</code> and <code>UnitSId</code> type values
   * in Sed is determined by an exact character sequence match; i.e.,
   * comparisons of these identifiers must be performed in a case-sensitive
   * manner.  This applies to all uses of <code>SId</code> and
   * <code>UnitSId</code>.
   *
   * @return the id of this Sed object.
   *
   * @note The fact that the value of attribute "id" is defined on the
   * SedBase parent class object is a convenience provided by libSed, and
   * <b>does not strictly follow Sed specifications</b>.  This libSed
   * implementation of SedBase allows client applications to use more
   * generalized code in some situations (for instance, when manipulating
   * objects that are all known to have identifiers), but beware that not
   * all Sed object classes provide an "id" attribute.  LibSed will allow
   * the identifier to be set, but it will not read nor write "id"
   * attributes for objects that do not possess them according to the Sed
   * specification for the Level and Version in use.
   *
   * @see setId(const std::string& sid)
   * @see isSetId()
   * @see unsetId()
   */
  virtual const std::string& getId() const;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   *
   * Returns the value of the "name" attribute of this object, if it has one.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  In contrast to the "id" attribute, the "name" attribute is
   * optional and is not intended to be used for cross-referencing purposes
   * within a model.  Its purpose instead is to provide a human-readable
   * label for the component.  The data type of "name" is the type
   * <code>string</code> defined in XML Schema.  Sed imposes no
   * restrictions as to the content of "name" attributes beyond those
   * restrictions defined by the <code>string</code> type in XML Schema.
   *
   * The recommended practice for handling "name" is as follows.  If a
   * software tool has the capability for displaying the content of "name"
   * attributes, it should display this content to the user as a
   * component's label instead of the component's "id".  If the user
   * interface does not have this capability (e.g., because it cannot
   * display or use special characters in symbol names), or if the "name"
   * attribute is missing on a given component, then the user interface
   * should display the value of the "id" attribute instead.  (Script
   * language interpreters are especially likely to display "id" instead of
   * "name".)
   *
   * As a consequence of the above, authors of systems that automatically
   * generate the values of "id" attributes should be aware some systems
   * may display the "id"'s to the user.  Authors therefore may wish to
   * take some care to have their software create "id" values that are: (a)
   * reasonably easy for humans to type and read; and (b) likely to be
   * meaningful, for example by making the "id" attribute be an abbreviated
   * form of the name attribute value.
   *
   * An additional point worth mentioning is although there are
   * restrictions on the uniqueness of "id" values, there are no
   * restrictions on the uniqueness of "name" values in a model.  This
   * allows software packages leeway in assigning component identifiers.
   *
   * @return the name of this Sed object.
   *
   * @note The fact that the "name" attribute is defined on the SedBase parent
   * class object is a convenience provided by libSed, and <b>does not
   * strictly follow Sed specifications</b>.  This libSed implementation
   * of SedBase allows client applications to use more generalized code in
   * some situations (for instance, when manipulating objects that are all
   * known to have identifiers), but beware that not all Sed object
   * classes provide an "id" attribute.  LibSed will allow the identifier
   * to be set, but it will not read nor write "id" attributes for objects
   * that do not possess them according to the Sed specification for the
   * Level and Version in use.
   *
   * @see isSetName()
   * @see setName(const std::string& name)
   * @see unsetName()
   */
  virtual const std::string& getName() const;
  /** @endcond */


  /**
   * Returns the content of the "notes" subelement of this object as
   * a tree of XMLNode objects.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * The "notes" element content returned by this method will be in XML
   * form, but libSed does not provide an object model specifically for
   * the content of notes.  Callers will need to traverse the XML tree
   * structure using the facilities available on XMLNode and related
   * objects.  For an alternative method of accessing the notes, see
   * getNotesString().
   *
   * @return the content of the "notes" subelement of this Sed object as a
   * tree structure composed of XMLNode objects.
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  XMLNode* getNotes();


  /**
   * Returns the content of the "notes" subelement of this object as
   * a tree of XMLNode objects.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * The "notes" element content returned by this method will be in XML
   * form, but libSed does not provide an object model specifically for
   * the content of notes.  Callers will need to traverse the XML tree
   * structure using the facilities available on XMLNode and related
   * objects.  For an alternative method of accessing the notes, see
   * getNotesString().
   *
   * @return the content of the "notes" subelement of this Sed object as a
   * tree structure composed of XMLNode objects.
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  XMLNode* getNotes() const;


  /**
   * Returns the content of the "notes" subelement of this object as a
   * string.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * For an alternative method of accessing the notes, see getNotes(),
   * which returns the content as an XMLNode tree structure.  Depending on
   * an application's needs, one or the other method may be more
   * convenient.
   *
   * @return the content of the "notes" subelement of this Sed object as a
   * string.
   *
   * @see getNotes()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  std::string getNotesString();


  /**
   * Returns the content of the "notes" subelement of this object as a
   * string.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * For an alternative method of accessing the notes, see getNotes(),
   * which returns the content as an XMLNode tree structure.  Depending on
   * an application's needs, one or the other method may be more
   * convenient.
   *
   * @return the content of the "notes" subelement of this Sed object as a
   * string.
   *
   * @see getNotes()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  std::string getNotesString() const;


  /**
   * Returns the content of the "annotation" subelement of this object as
   * a tree of XMLNode objects.
   *
   * Whereas the Sed "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * The annotations returned by this method will be in XML form.  LibSed
   * provides an object model and related interfaces for certain specific
   * kinds of annotations, namely model history information and RDF
   * content.  See the ModelHistory, CVTerm and RDFAnnotationParser classes
   * for more information about the facilities available.
   *
   * @return the annotation of this Sed object as a tree of XMLNode objects.
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  XMLNode* getAnnotation();


  /**
   * Returns the content of the "annotation" subelement of this object as
   * a tree of XMLNode objects.
   *
   * Whereas the Sed "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * The annotations returned by this method will be in XML form.  LibSed
   * provides an object model and related interfaces for certain specific
   * kinds of annotations, namely model history information and RDF
   * content.  See the ModelHistory, CVTerm and RDFAnnotationParser classes
   * for more information about the facilities available.
   *
   * @return the annotation of this Sed object as a tree of XMLNode objects.
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  XMLNode* getAnnotation() const;


  /**
   * Returns the content of the "annotation" subelement of this object as a
   * character string.
   *
   * Whereas the Sed "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * The annotations returned by this method will be in string form.
   *
   * @return the annotation of this Sed object as a character string.
   *
   * @see getAnnotation()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  std::string getAnnotationString();


  /**
   * Returns the content of the "annotation" subelement of this object as a
   * character string.
   *
   * Whereas the Sed "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * The annotations returned by this method will be in string form.
   *
   * @return the annotation of this Sed object as a character string.
   *
   * @see getAnnotation()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  std::string getAnnotationString() const;


  /**
   * Returns a list of the XML Namespaces declared on this Sed document.
   *
   * The SedNamespaces object encapsulates Sed Level/Version/namespaces
   * information.  It is used to communicate the Sed Level, Version, and
   * (in Sed Level&nbsp;3) packages used in addition to Sed Level&nbsp;3
   * Core.
   *
   * @return the XML Namespaces associated with this Sed object
   *
   * @see getLevel()
   * @see getVersion()
   */
  virtual XMLNamespaces* getNamespaces() const ;


  /**
   * Returns the SedDocument object containing @em this object instance.
   *
   * LibSed uses the class SedDocument as a top-level container for
   * storing Sed content and data associated with it (such as warnings and
   * error messages).  An Sed model in libSed is contained inside an
   * SedDocument object.  SedDocument corresponds roughly to the class
   * <i>Sed</i> defined in the Sed Level&nbsp;3 and Level&nbsp;2
   * specifications, but it does not have a direct correspondence in Sed
   * Level&nbsp;1.  (But, it is created by libSed no matter whether the
   * model is Level&nbsp;1, Level&nbsp;2 or Level&nbsp;3.)
   *
   * This method allows the caller to obtain the SedDocument for the
   * current object.
   *
   * @return the parent SedDocument object of this Sed object.
   *
   * @see getParentSedObject()
   * @see getModel()
   */
  const SedDocument* getSedDocument() const;


  /**
   * Returns the SedDocument object containing @em this object instance.
   *
   * LibSed uses the class SedDocument as a top-level container for
   * storing Sed content and data associated with it (such as warnings and
   * error messages).  An Sed model in libSed is contained inside an
   * SedDocument object.  SedDocument corresponds roughly to the class
   * <i>Sed</i> defined in the Sed Level&nbsp;3 and Level&nbsp;2
   * specifications, but it does not have a direct correspondence in Sed
   * Level&nbsp;1.  (But, it is created by libSed no matter whether the
   * model is Level&nbsp;1, Level&nbsp;2 or Level&nbsp;3.)
   *
   * This method allows the caller to obtain the SedDocument for the
   * current object.
   *
   * @return the parent SedDocument object of this Sed object.
   *
   * @see getParentSedObject()
   * @see getModel()
   */
  SedDocument* getSedDocument();


  /**
   * Returns the parent Sed object containing this object.
   *
   * This returns the immediately-containing object.  This method is
   * convenient when holding an object nested inside other objects in an
   * Sed model.
   *
   * @return the parent Sed object of this Sed object.
   *
   * @see getSedDocument()
   * @see getModel()
   */
  SedBase* getParentSedObject();


  /**
   * Returns the parent Sed object containing this object.
   *
   * This returns the immediately-containing object.  This method is
   * convenient when holding an object nested inside other objects in an
   * Sed model.
   *
   * @return the parent Sed object of this Sed object.
   *
   * @see getSedDocument()
   * @see getModel()
   */
  const SedBase* getParentSedObject() const;


  /**
   * Returns the first ancestor object that has the given Sed type code.
   *
   * @if clike LibSed attaches an identifying code to every kind of Sed
   * object.  These are known as <em>Sed type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SedTypeCode_t.
   * The names of the type codes all begin with the characters @c
   *SEDML_. @endif@if java LibSed attaches an identifying code to every
   * kind of Sed object.  These are known as <em>Sed type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSed, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @cSEDML_. @endif@if python LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the Python language interface for libSed, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @cSEDML_. @endif@if csharp LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the C# language interface for libSed, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml libsbml@endlink.  The names of the type codes all begin with
   * the characters @cSEDML_. @endif@~
   *
   * This method searches the tree of objects that are parents of this
   * object, and returns the first one that has the given Sed type code.
   * If the optional argument @p pkgName is given, it will cause the search
   * to be limited to the Sed Level&nbsp;3 package given.
   *
   * @param type the Sed type code of the object sought
   *
   * @param pkgName (optional) the short name of an Sed Level&nbsp;3
   * package to which the sought-after object must belong
   *
   * @return the ancestor Sed object of this Sed object that corresponds
   * to the given @if clike #SedTypeCode_t value@else Sed object type
   * code@endif, or @c NULL if no ancestor exists.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  SedBase* getAncestorOfType(int type, const std::string pkgName = "core");


  /**
   * Returns the first ancestor object that has the given Sed type code.
   *
   * @if clike LibSed attaches an identifying code to every kind of Sed
   * object.  These are known as <em>Sed type codes</em>.  The set of
   * possible type codes is defined in the enumeration #SedTypeCode_t.
   * The names of the type codes all begin with the characters @c
   *SEDML_. @endif@if java LibSed attaches an identifying code to every
   * kind of Sed object.  These are known as <em>Sed type codes</em>.  In
   * other languages, the set of type codes is stored in an enumeration; in
   * the Java language interface for libSed, the type codes are defined as
   * static integer constants in the interface class {@link
   * libsbmlConstants}.  The names of the type codes all begin with the
   * characters @cSEDML_. @endif@if python LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the Python language interface for libSed, the type
   * codes are defined as static integer constants in the interface class
   * @link libsbml@endlink.  The names of the type codes all begin with the
   * characters @cSEDML_. @endif@if csharp LibSed attaches an identifying
   * code to every kind of Sed object.  These are known as <em>Sed type
   * codes</em>.  In the C# language interface for libSed, the type codes
   * are defined as static integer constants in the interface class @link
   * libsbmlcs.libsbml libsbml@endlink.  The names of the type codes all begin with
   * the characters @cSEDML_. @endif@~
   *
   * This method searches the tree of objects that are parents of this
   * object, and returns the first one that has the given Sed type code.
   * If the optional argument @p pkgName is given, it will cause the search
   * to be limited to the Sed Level&nbsp;3 package given.
   *
   * @param type the Sed type code of the object sought
   *
   * @param pkgName (optional) the short name of an Sed Level&nbsp;3
   * package to which the sought-after object must belong
   *
   * @return the ancestor Sed object of this Sed object that corresponds
   * to the given @if clike #SedTypeCode_t value@else Sed object type
   * code@endif, or @c NULL if no ancestor exists.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  const SedBase* getAncestorOfType(int type, const std::string pkgName = "core") const;


  /**
   * Returns the line number on which this object first appears in the XML
   * representation of the Sed document.
   *
   * @return the line number of this Sed object.
   *
   * @note The line number for each construct in an Sed model is set upon
   * reading the model.  The accuracy of the line number depends on the
   * correctness of the XML representation of the model, and on the
   * particular XML parser library being used.  The former limitation
   * relates to the following problem: if the model is actually invalid
   * XML, then the parser may not be able to interpret the data correctly
   * and consequently may not be able to establish the real line number.
   * The latter limitation is simply that different parsers seem to have
   * their own accuracy limitations, and out of all the parsers supported
   * by libSed, none have been 100% accurate in all situations. (At this
   * time, libSed supports the use of <a target="_blank"
   * href="http://xmlsoft.org">libxml2</a>, <a target="_blank"
   * href="http://expat.sourceforge.net/">Expat</a> and <a target="_blank"
   * href="http://xerces.apache.org/xerces-c/">Xerces</a>.)
   *
   * @see getColumn()
   */
  unsigned int getLine() const;


  /**
   * Returns the column number on which this object first appears in the XML
   * representation of the Sed document.
   *
   * @return the column number of this Sed object.
   *
   * @note The column number for each construct in an Sed model is set
   * upon reading the model.  The accuracy of the column number depends on
   * the correctness of the XML representation of the model, and on the
   * particular XML parser library being used.  The former limitation
   * relates to the following problem: if the model is actually invalid
   * XML, then the parser may not be able to interpret the data correctly
   * and consequently may not be able to establish the real column number.
   * The latter limitation is simply that different parsers seem to have
   * their own accuracy limitations, and out of all the parsers supported
   * by libSed, none have been 100% accurate in all situations. (At this
   * time, libSed supports the use of <a target="_blank"
   * href="http://xmlsoft.org">libxml2</a>, <a target="_blank"
   * href="http://expat.sourceforge.net/">Expat</a> and <a target="_blank"
   * href="http://xerces.apache.org/xerces-c/">Xerces</a>.)
   *
   * @see getLine()
   */
  unsigned int getColumn() const;

  /**
   * Predicate returning @c true if this object's "metaid" attribute is set.
   *
   * The optional attribute named "metaid", present on every major Sed
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an Sed file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by Sed&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model component for purposes
   * such as referencing that component from metadata placed within
   * "annotation" subelements.
   *
   * @return @c true if the "metaid" attribute of this Sed object is
   * set, @c false otherwise.
   *
   * @see getMetaId()
   * @see setMetaId(const std::string& metaid)
   */
  bool isSetMetaId() const;


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   * Predicate returning @c true if this
   * object's "id" attribute is set.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  The identifier given by an object's "id" attribute value
   * is used to identify the object within the Sed model definition.
   * Other objects can refer to the component using this identifier.  The
   * data type of "id" is always either <code>Sid</code> or
   * <code>UnitSId</code>, depending on the object in question.  Both
   * data types are defined as follows:
   * @verbatim
  letter ::= 'a'..'z','A'..'Z'
  digit  ::= '0'..'9'
  idChar ::= letter | digit | '_'
  SId    ::= ( letter | '_' ) idChar*
  @endverbatim
   *
   * The equality of <code>SId</code> and <code>UnitSId</code> type values
   * in Sed is determined by an exact character sequence match; i.e.,
   * comparisons of these identifiers must be performed in a case-sensitive
   * manner.  This applies to all uses of <code>SId</code> and
   * <code>UnitSId</code>.
   *
   * @return @c true if the "id" attribute of this Sed object is
   * set, @c false otherwise.
   *
   * @note The fact that the value of attribute "id" is defined on the
   * SedBase parent class object is a convenience provided by libSed, and
   * <b>does not strictly follow Sed specifications</b>.  This libSed
   * implementation of SedBase allows client applications to use more
   * generalized code in some situations (for instance, when manipulating
   * objects that are all known to have identifiers), but beware that not
   * all Sed object classes provide an "id" attribute.  LibSed will allow
   * the identifier to be set, but it will not read nor write "id"
   * attributes for objects that do not possess them according to the Sed
   * specification for the Level and Version in use.
   *
   * @see getId()
   * @see setId(const std::string& sid)
   * @see unsetId()
   */
  virtual bool isSetId() const;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   * Predicate returning @c true if this
   * object's "name" attribute is set.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  In contrast to the "id" attribute, the "name" attribute is
   * optional and is not intended to be used for cross-referencing purposes
   * within a model.  Its purpose instead is to provide a human-readable
   * label for the component.  The data type of "name" is the type
   * <code>string</code> defined in XML Schema.  Sed imposes no
   * restrictions as to the content of "name" attributes beyond those
   * restrictions defined by the <code>string</code> type in XML Schema.
   *
   * The recommended practice for handling "name" is as follows.  If a
   * software tool has the capability for displaying the content of "name"
   * attributes, it should display this content to the user as a
   * component's label instead of the component's "id".  If the user
   * interface does not have this capability (e.g., because it cannot
   * display or use special characters in symbol names), or if the "name"
   * attribute is missing on a given component, then the user interface
   * should display the value of the "id" attribute instead.  (Script
   * language interpreters are especially likely to display "id" instead of
   * "name".)
   *
   * As a consequence of the above, authors of systems that automatically
   * generate the values of "id" attributes should be aware some systems
   * may display the "id"'s to the user.  Authors therefore may wish to
   * take some care to have their software create "id" values that are: (a)
   * reasonably easy for humans to type and read; and (b) likely to be
   * meaningful, for example by making the "id" attribute be an abbreviated
   * form of the name attribute value.
   *
   * An additional point worth mentioning is although there are
   * restrictions on the uniqueness of "id" values, there are no
   * restrictions on the uniqueness of "name" values in a model.  This
   * allows software applications leeway in assigning component identifiers.
   *
   * @return @c true if the "name" attribute of this Sed object is
   * set, @c false otherwise.
   *
   * @note The fact that the "name" attribute is defined on the SedBase parent
   * class object is a convenience provided by libSed, and <b>does not
   * strictly follow Sed specifications</b>.  This libSed implementation
   * of SedBase allows client applications to use more generalized code in
   * some situations (for instance, when manipulating objects that are all
   * known to have identifiers), but beware that not all Sed object
   * classes provide an "id" attribute.  LibSed will allow the identifier
   * to be set, but it will not read nor write "id" attributes for objects
   * that do not possess them according to the Sed specification for the
   * Level and Version in use.
   *
   * @see getName()
   * @see setName(const std::string& name)
   * @see unsetName()
   */
  virtual bool isSetName() const;
  /** @endcond */


  /**
   * Predicate returning @c true if this
   * object's "notes" subelement exists and has content.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * @return @c true if a "notes" subelement exists, @c false otherwise.
   *
   * @see getNotes()
   * @see getNotesString()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  bool isSetNotes() const;


  /**
   * Predicate returning @c true if this
   * object's "annotation" subelement exists and has content.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * @return @c true if a "annotation" subelement exists, @c false
   * otherwise.
   *
   * @see getAnnotation()
   * @see getAnnotationString()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  bool isSetAnnotation() const;

  /**
   * Sets the value of the "metaid" attribute of this object.
   *
   * The string @p metaid is copied.  The value of @p metaid must be an
   * identifier conforming to the syntax defined by the XML 1.0 data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">ID</a>.  Among other
   * things, this type requires that a value is unique among all the values
   * of type XML ID in an SedDocument.  Although Sed only uses <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a> for the "metaid"
   * attribute, callers should be careful if they use
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>'s in XML
   * portions of a model that are not defined by Sed, such as in the
   * application-specific content of the "annotation" subelement.
   *
   * @param metaid the identifier string to use as the value of the
   * "metaid" attribute
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_ATTRIBUTE_VALUE LIBSEDML_INVALID_ATTRIBUTE_VALUE @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_UNEXPECTED_ATTRIBUTE LIBSEDML_UNEXPECTED_ATTRIBUTE @endlink
   *
   * @see getMetaId()
   * @see isSetMetaId()
   */
  int setMetaId(const std::string& metaid);


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   *
   * Sets the value of the "id" attribute of this Sed object to a copy
   * of @p id.
   *
   * The string @p sid is copied.  Note that Sed has strict requirements
   * for the syntax of identifiers.  @htmlinclude id-syntax.html
   *
   * @param sid the string to use as the identifier of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_ATTRIBUTE_VALUE LIBSEDML_INVALID_ATTRIBUTE_VALUE @endlink
   */
  virtual int setId(const std::string& sid);
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /*
   * NOTE: THIS IS FOR BACKWARD COMPATABILITY REASONS
   * Sets the value of the "name" attribute of this Sed object to a copy
   * of @p name.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  In contrast to the "id" attribute, the "name" attribute is
   * optional and is not intended to be used for cross-referencing purposes
   * within a model.  Its purpose instead is to provide a human-readable
   * label for the component.  The data type of "name" is the type
   * <code>string</code> defined in XML Schema.  Sed imposes no
   * restrictions as to the content of "name" attributes beyond those
   * restrictions defined by the <code>string</code> type in XML Schema.
   *
   * The recommended practice for handling "name" is as follows.  If a
   * software tool has the capability for displaying the content of "name"
   * attributes, it should display this content to the user as a
   * component's label instead of the component's "id".  If the user
   * interface does not have this capability (e.g., because it cannot
   * display or use special characters in symbol names), or if the "name"
   * attribute is missing on a given component, then the user interface
   * should display the value of the "id" attribute instead.  (Script
   * language interpreters are especially likely to display "id" instead of
   * "name".)
   *
   * As a consequence of the above, authors of systems that automatically
   * generate the values of "id" attributes should be aware some systems
   * may display the "id"'s to the user.  Authors therefore may wish to
   * take some care to have their software create "id" values that are: (a)
   * reasonably easy for humans to type and read; and (b) likely to be
   * meaningful, for example by making the "id" attribute be an abbreviated
   * form of the name attribute value.
   *
   * An additional point worth mentioning is although there are
   * restrictions on the uniqueness of "id" values, there are no
   * restrictions on the uniqueness of "name" values in a model.  This
   * allows software applications leeway in assigning component identifiers.
   *
   * @param name the new name for the object; the string will be copied
   *
   * @note The fact that the "name" attribute is defined on the SedBase parent
   * class object is a convenience provided by libSed, and <b>does not
   * strictly follow Sed specifications</b>.  This libSed implementation
   * of SedBase allows client applications to use more generalized code in
   * some situations (for instance, when manipulating objects that are all
   * known to have identifiers), but beware that not all Sed object
   * classes provide an "id" attribute.  LibSed will allow the identifier
   * to be set, but it will not read nor write "id" attributes for objects
   * that do not possess them according to the Sed specification for the
   * Level and Version in use.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_ATTRIBUTE_VALUE LIBSEDML_INVALID_ATTRIBUTE_VALUE @endlink
   *
   * @see getName()
   * @see isSetName()
   * @see unsetName()
   */
  virtual int setName(const std::string& name);
  /** @endcond */


  /**
   * Sets the value of the "annotation" subelement of this Sed object.
   *
   * The content of @p annotation is copied, and any previous content of
   * this object's "annotation" subelement is deleted.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * Call this method will result in any existing content of the
   * "annotation" subelement to be discarded.  Unless you have taken steps
   * to first copy and reconstitute any existing annotations into the @p
   * annotation that is about to be assigned, it is likely that performing
   * such wholesale replacement is unfriendly towards other software
   * applications whose annotations are discarded.  An alternative may be
   * to use SedBase::appendAnnotation(const XMLNode* annotation) or
   * SedBase::appendAnnotation(const std::string& annotation).
   *
   * @param annotation an XML structure that is to be used as the new content
   * of the "annotation" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  virtual int setAnnotation(const XMLNode* annotation);


  /**
   * Sets the value of the "annotation" subelement of this Sed object.
   *
   * The content of @p annotation is copied, and any previous content of
   * this object's "annotation" subelement is deleted.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * Call this method will result in any existing content of the
   * "annotation" subelement to be discarded.  Unless you have taken steps
   * to first copy and reconstitute any existing annotations into the @p
   * annotation that is about to be assigned, it is likely that performing
   * such wholesale replacement is unfriendly towards other software
   * applications whose annotations are discarded.  An alternative may be
   * to use SedBase::appendAnnotation(const XMLNode* annotation) or
   * SedBase::appendAnnotation(const std::string& annotation).
   *
   * @param annotation an XML string that is to be used as the content
   * of the "annotation" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  virtual int setAnnotation(const std::string& annotation);


  /**
   * Appends the given @p annotation to the "annotation" subelement of this
   * object.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a
   * target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type "any"</a>,
   * allowing essentially arbitrary well-formed XML data content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * Unlike SedBase::setAnnotation(const XMLNode* annotation) or
   * SedBase::setAnnotation(const std::string& annotation), this method
   * allows other annotations to be preserved when an application adds its
   * own data.
   *
   * @param annotation an XML structure that is to be copied and appended
   * to the content of the "annotation" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const std::string& annotation)
   * @see unsetAnnotation()
   */
  virtual int appendAnnotation(const XMLNode* annotation);


  /**
   * Appends the given @p annotation to the "annotation" subelement of this
   * object.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a
   * target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type "any"</a>,
   * allowing essentially arbitrary well-formed XML data content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * Unlike SedBase::setAnnotation(const XMLNode* annotation) or
   * SedBase::setAnnotation(const std::string& annotation), this method
   * allows other annotations to be preserved when an application adds its
   * own data.
   *
   * @param annotation an XML string that is to be copied and appended
   * to the content of the "annotation" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see unsetAnnotation()
   */
  virtual int appendAnnotation(const std::string& annotation);


  /**
   * Removes the top-level element within the "annotation" subelement of this
   * Sed object with the given name and optional URI.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * Calling this method allows a particular annotation element to be removed
   * whilst the remaining annotations remain intact.
   *
   * @param elementName a string representing the name of the top level
   * annotation element that is to be removed
   * @param elementURI an optional string that is used to check both the name
   * and URI of the top level element to be removed
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_ANNOTATION_NAME_NOT_FOUND LIBSEDML_ANNOTATION_NAME_NOT_FOUND @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_ANNOTATION_NS_NOT_FOUND LIBSEDML_ANNOTATION_NS_NOT_FOUND @endlink
   *
   * @see replaceTopLevelAnnotationElement(const XMLNode *)
   * @see replaceTopLevelAnnotationElement(const std::string&)
   */
  int removeTopLevelAnnotationElement(const std::string elementName,
                                      const std::string elementURI = "");


  /**
   * Replaces the given top-level element within the "annotation"
   * subelement of this Sed object and with the annotation element supplied.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * This method determines the name of the element to be replaced from the
   * annotation argument. Functionally it is equivalent to calling
   * <code> removeTopLevelAnnotationElement(name); appendAnnotation(annotation_with_name);
   * </code> with the exception that the placement of the annotation element remains
   * the same.
   *
   * @param annotation XMLNode representing the replacement top level annotation
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   *
   * @see removeTopLevelAnnotationElement(const std::string elementName, const std::string elementURI)
   * @see replaceTopLevelAnnotationElement(const std::string&)
   */
  int replaceTopLevelAnnotationElement(const XMLNode* annotation);


  /**
   * Replaces the given top-level element within the "annotation"
   * subelement of this Sed object and with the annotation element supplied.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * This method determines the name of the element to be replaced from the
   * annotation argument. Functionally it is equivalent to calling
   * <code> removeTopLevelAnnotationElement(name); appendAnnotation(annotation_with_name);
   * </code> with the exception that the placement of the annotation element remains
   * the same.
   *
   * @param annotation string representing the replacement top level annotation
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   *
   * @see removeTopLevelAnnotationElement(const std::string elementName, const std::string elementURI)
   * @see replaceTopLevelAnnotationElement(const XMLNode*)
   */
  int replaceTopLevelAnnotationElement(const std::string& annotation);


  /**
   * Sets the value of the "notes" subelement of this Sed object.
   *
   * The content of @p notes is copied, and any existing content of this
   * object's "notes" subelement is deleted.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * @param notes an XML structure that is to be used as the content of the
   * "notes" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  int setNotes(const XMLNode* notes);


  /**
   * Sets the value of the "notes" subelement of this Sed object to a copy
   * of the string @p notes.
   *
   * The content of @p notes is copied, and any existing content of this
   * object's "notes" subelement is deleted.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * The following code illustrates a very simple way of setting the notes
   * using this method.  Here, the object being annotated is the whole Sed
   * document, but that is for illustration purposes only; you could of
   * course use this same approach to annotate any other Sed component.
   * @if clike
  @verbatim
  SedDocument* s = new SedDocument(3, 1);
  s->setNotes("<body xmlns='http://www.w3.org/1999/xhtml'><p>here is my note</p></body>");
  @endverbatim
   * @endif@if java
  @verbatim
  SedDocument s = new SedDocument(3, 1);
  s.setNotes("<body xmlns='http://www.w3.org/1999/xhtml'><p>here is my note</p></body>");
  @endverbatim
   * @endif@if csharp
  @verbatim
  SedDocument s = new SedDocument(3, 1);
  s.setNotes("<body xmlns='http://www.w3.org/1999/xhtml'><p>here is my note</p></body>");
  @endverbatim
   * @endif@~
   *
   * @param notes an XML string that is to be used as the content of the
   * "notes" subelement of this object
   *
   * @param addXHTMLMarkup a boolean indicating whether to wrap the contents
   * of the @p notes argument with XHTML paragraph (<code>&lt;p&gt;</code>)
   * tags.  This is appropriate when the string in @p notes does not already
   * containg the appropriate XHTML markup.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  int setNotes(const std::string& notes, bool addXHTMLMarkup = false);


  /**
   * Appends the given @p notes to the "notes" subelement of this object.
   *
   * The content of @p notes is copied.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * @param notes an XML node structure that is to appended to the content
   * of the "notes" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const std::string& notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  int appendNotes(const XMLNode* notes);


  /**
   * Appends the given @p notes to the "notes" subelement of this object.
   *
   * The content of the parameter @p notes is copied.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * @param notes an XML string that is to appended to the content of
   * the "notes" subelement of this object
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_OBJECT LIBSEDML_INVALID_OBJECT @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see unsetNotes()
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  int appendNotes(const std::string& notes);


  /** @cond doxygen-libsbml-internal */
  /**
   * Sets the parent SedDocument of this Sed object.
   *
   * @param d the SedDocument object to use
   *
   * @see connectToChild()
   * @if clike
   * @see enablePackageInternal()
   * @endif@~
   */
  virtual void setSedDocument(SedDocument* d);
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /**
   * Sets the parent Sed object of this Sed object.
   * (Creates a child-parent relationship by the child)
   * This function is called when a child element is
   * set/added/created by its parent element (e.g. by setXXX,
   * addXXX, createXXX, and connectToChild functions of the
   * parent element).
   *
   * @param parent the Sed object to use
   */
  virtual void connectToParent(SedBase* parent);


  /**
   * Sets this Sed object to child Sed objects (if any).
   * (Creates a child-parent relationship by the parent)
   *
   * Subclasses must override this function if they define
   * one ore more child elements.
   * Basically, this function needs to be called in
   * constructor, copy constructor, assignment operator.
   *
   * @if clike
   * @see setSedDocument()
   * @see enablePackageInternal()
   * @endif@~
   */
  virtual void connectToChild();

  /** @endcond */


  /**
   * Sets the namespaces relevant of this Sed object.
   *
   * The content of @p xmlns is copied, and this object's existing
   * namespace content is deleted.
   *
   * The SedNamespaces object encapsulates Sed Level/Version/namespaces
   * information.  It is used to communicate the Sed Level, Version, and
   * (in Level&nbsp;3) packages used in addition to Sed Level&nbsp;3 Core.
   *
   * @param xmlns the namespaces to set
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   */
  int setNamespaces(XMLNamespaces* xmlns);


  /**
   * Unsets the value of the "metaid" attribute of this Sed object.
   *
   * The optional attribute named "metaid", present on every major Sed
   * component type, is for supporting metadata annotations using RDF
   * (Resource Description Format). The attribute value has the data type
   * <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, the XML
   * identifier type, which means each "metaid" value must be globally
   * unique within an Sed file.  (Importantly, this uniqueness criterion
   * applies across any attribute with type <a
   * href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>, not just the
   * "metaid" attribute used by Sed&mdash;something to be aware of if your
   * application-specific XML content inside the "annotation" subelement
   * happens to use <a href="http://www.w3.org/TR/REC-xml/#id">XML ID</a>.)
   * The "metaid" value serves to identify a model component for purposes
   * such as referencing that component from metadata placed within
   * "annotation" subelements.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_UNEXPECTED_ATTRIBUTE LIBSEDML_UNEXPECTED_ATTRIBUTE @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   */
  int unsetMetaId();


  /**
   * Unsets the value of the "id" attribute of this Sed object.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  The identifier given by an object's "id" attribute value
   * is used to identify the object within the Sed model definition.
   * Other objects can refer to the component using this identifier.  The
   * data type of "id" is always either <code>Sid</code> or
   * <code>UnitSId</code>, depending on the object in question.  Both
   * data types are defined as follows:
   * <pre style="margin-left: 2em; border: none; font-weight: bold; color: black">
   *   letter ::= 'a'..'z','A'..'Z'
   *   digit  ::= '0'..'9'
   *   idChar ::= letter | digit | '_'
   *   SId    ::= ( letter | '_' ) idChar*
   * </pre>
   *
   * The equality of <code>SId</code> and <code>UnitSId</code> type values
   * in Sed is determined by an exact character sequence match; i.e.,
   * comparisons of these identifiers must be performed in a case-sensitive
   * manner.  This applies to all uses of <code>SId</code> and
   * <code>UnitSId</code>.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   */
  virtual int unsetId();


  /**
   * Unsets the value of the "name" attribute of this Sed object.
   *
   * Most (but not all) objects in Sed include two common attributes: "id"
   * and "name".  In contrast to the "id" attribute, the "name" attribute is
   * optional and is not intended to be used for cross-referencing purposes
   * within a model.  Its purpose instead is to provide a human-readable
   * label for the component.  The data type of "name" is the type
   * <code>string</code> defined in XML Schema.  Sed imposes no
   * restrictions as to the content of "name" attributes beyond those
   * restrictions defined by the <code>string</code> type in XML Schema.
   *
   * The recommended practice for handling "name" is as follows.  If a
   * software tool has the capability for displaying the content of "name"
   * attributes, it should display this content to the user as a
   * component's label instead of the component's "id".  If the user
   * interface does not have this capability (e.g., because it cannot
   * display or use special characters in symbol names), or if the "name"
   * attribute is missing on a given component, then the user interface
   * should display the value of the "id" attribute instead.  (Script
   * language interpreters are especially likely to display "id" instead of
   * "name".)
   *
   * As a consequence of the above, authors of systems that automatically
   * generate the values of "id" attributes should be aware some systems
   * may display the "id"'s to the user.  Authors therefore may wish to
   * take some care to have their software create "id" values that are: (a)
   * reasonably easy for humans to type and read; and (b) likely to be
   * meaningful, for example by making the "id" attribute be an abbreviated
   * form of the name attribute value.
   *
   * An additional point worth mentioning is although there are
   * restrictions on the uniqueness of "id" values, there are no
   * restrictions on the uniqueness of "name" values in a model.  This
   * allows software applications leeway in assigning component identifiers.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   */
  virtual int unsetName();


  /**
   * Unsets the value of the "notes" subelement of this Sed object.
   *
   * The optional Sed element named "notes", present on every major Sed
   * component type, is intended as a place for storing optional
   * information intended to be seen by humans.  An example use of the
   * "notes" element would be to contain formatted user comments about the
   * model element in which the "notes" element is enclosed.  Every object
   * derived directly or indirectly from type SedBase can have a separate
   * value for "notes", allowing users considerable freedom when adding
   * comments to their models.
   *
   * The format of "notes" elements must be <a target="_blank"
   * href="http://www.w3.org/TR/xhtml1/">XHTML&nbsp;1.0</a>.  To help
   * verify the formatting of "notes" content, libSed provides the static
   * utility method SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif); however,
   * readers are urged to consult the appropriate <a target="_blank"
   * href="http://sbml.org/Documents/Specifications">Sed specification
   * document</a> for the Level and Version of their model for more
   * in-depth explanations.  The Sed Level&nbsp;2 and &nbsp;3
   * specifications have considerable detail about how "notes" element
   * content must be structured.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   *
   * @see getNotesString()
   * @see isSetNotes()
   * @see setNotes(const XMLNode* notes)
   * @see setNotes(const std::string& notes)
   * @see appendNotes(const XMLNode* notes)
   * @see appendNotes(const std::string& notes)
   * @see SyntaxChecker::hasExpectedXHTMLSyntax(@if java XMLNode xhtml@endif)
   */
  int unsetNotes();


  /**
   * Unsets the value of the "annotation" subelement of this Sed object.
   *
   * Whereas the SedBase "notes" subelement is a container for content to be
   * shown directly to humans, the "annotation" element is a container for
   * optional software-generated content @em not meant to be shown to
   * humans.  Every object derived from SedBase can have its own value for
   * "annotation".  The element's content type is <a target="_blank"
   * href="http://www.w3.org/TR/2004/REC-xml-20040204/#elemdecls">XML type
   * "any"</a>, allowing essentially arbitrary well-formed XML data
   * content.
   *
   * Sed places a few restrictions on the organization of the content of
   * annotations; these are intended to help software tools read and write
   * the data as well as help reduce conflicts between annotations added by
   * different tools.  Please see the Sed specifications for more details.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   *
   * @see getAnnotation()
   * @see getAnnotationString()
   * @see isSetAnnotation()
   * @see setAnnotation(const XMLNode* annotation)
   * @see setAnnotation(const std::string& annotation)
   * @see appendAnnotation(const XMLNode* annotation)
   * @see appendAnnotation(const std::string& annotation)
   */
  int unsetAnnotation();


  /**
   * Returns the Sed Level of the SedDocument object containing this
   * object.
   *
   * @return the Sed level of this Sed object.
   *
   * @see getVersion()
   * @see getNamespaces()
   * @see getPackageVersion()
   */
  virtual const int getLevel() const;


  /**
   * Returns the Version within the Sed Level of the SedDocument object
   * containing this object.
   *
   * @return the Sed version of this Sed object.
   *
   * @see getLevel()
   * @see getNamespaces()
   */
  virtual const int getVersion() const;


  /**
   * Returns the libSed type code for this object.
   *
   * This method may return the type code of this Sed object, or it may
   * return @link SedTypeCode_t#Sed_UNKNOWNSEDML_UNKNOWN@endlink.  This
   * is because subclasses of SedBase are not required to implement this
   * method to return a type code.  This method is meant primarily for the
   * LibSed C interface, in which class and subclass information is not
   * readily available.
   *
   * @return the @if clike #SedTypeCode_t value@else Sed object type code@endif@~
   * of this Sed object or
   * @link SedTypeCode_t#Sed_UNKNOWNSEDML_UNKNOWN@endlink (the default).
   *
   * @see getElementName()
   * @see getPackageName()
   */
  virtual int getTypeCode() const;


  /**
   * Predicate returning @c true if this
   * object's level/version and namespace values correspond to a valid
   * Sed specification.
   *
   * @return @c true if the level, version and namespace values of this
   * Sed object correspond to a valid set of values, @c false otherwise.
   */
  bool hasValidLevelVersionNamespaceCombination();


  /**
   * Returns the XML element name of this object.
   *
   * This is overridden by subclasses to return a string appropriate to the
   * Sed component.  For example, Model defines it as returning @c
   * "model", CompartmentType defines it as returning @c "compartmentType",
   * and so on.
   */
  virtual const std::string& getElementName() const = 0;


  /**
   * Returns a string consisting of a partial Sed corresponding to just
   * this object.
   *
   * @return the partial Sed that describes this Sed object.
   *
   * @warning This is primarily provided for testing and debugging
   * purposes.  It may be removed in a future version of libSed.
   */
  char* toSed();


  /** @cond doxygen-libsbml-internal */
  /**
   * Reads (initializes) this Sed object by reading from XMLInputStream.
   */
  void read(XMLInputStream& stream);
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /**
   * Writes (serializes) this Sed object by writing it to XMLOutputStream.
   */
  void write(XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /**
   * Subclasses should override this method to write out their contained
   * Sed objects as XML elements.  Be sure to call your parents
   * implementation of this method as well.  For example:@if clike
   * <pre>
   *   SedBase::writeElements();
   *   mReactans.write(stream);
   *   mProducts.write(stream);
   *   ...
   * </pre>@endif@~
   */
  virtual void writeElements(XMLOutputStream& stream) const;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* function returns true if component has all the required
   * attributes
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredAttributes() const ;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* function returns true if component has all the required
   * elements
   * needs to be overloaded for each component
   */
  virtual bool hasRequiredElements() const ;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* function returns value if the object matches level/version/namespace
   */
  int checkCompatibility(const SedBase * object) const ;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* sets the Sednamespaces - internal use only*/
  int setSedNamespaces(SedNamespaces * sbmlns);

  /* sets the SedNamaepaces and owns the given object - internal use only */
  void setSedNamespacesAndOwn(SedNamespaces * sbmlns);
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* gets the Sednamespaces - internal use only*/
  virtual SedNamespaces * getSedNamespaces() const;
  /** @endcond */


  /** @cond doxygen-libsbml-internal */
  /* removes duplicate top level annotations*/
  void removeDuplicateAnnotations();
  const std::string checkMathMLNamespace(const XMLToken elem);
  /** @endcond */


  /**
   * Removes itself from its parent.  If the parent was storing it as a
   * pointer, it is deleted.  If not, it is simply cleared (as in ListOf
   * objects).  Pure virutal, as every SedBase element has different parents,
   * and therefore different methods of removing itself.  Will fail (and
   * not delete itself) if it has no parent object.  This function is
   * designed to be overridden, but for all objects whose parent is of the
   * class ListOf, the default implementation will work.
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
   * Returns @c true if this object's set of XML namespaces are the same
   * as the given object's XML namespaces.
   *
   * @param sb an object to compare with respect to namespaces
   *
   * @return boolean, @c true if this object's collection of namespaces is
   * the same as @p sb's, @c false otherwise.
   */
  bool matchesSedNamespaces(const SedBase * sb);


  /**
   * Returns @c true if this object's set of XML namespaces are the same
   * as the given object's XML namespaces.
   *
   * @param sb an object to compare with respect to namespaces
   *
   * @return boolean, @c true if this object's collection of namespaces is
   * the same as @p sb's, @c false otherwise.
   */
  bool matchesSedNamespaces(const SedBase * sb) const;


  /**
   * Returns @c true if this object's set of XML namespaces are a subset
   * of the given object's XML namespaces.
   *
   * @param sb an object to compare with respect to namespaces
   *
   * @return boolean, @c true if this object's collection of namespaces is
   * a subset of @p sb's, @c false otherwise.
   */
  bool matchesRequiredSedNamespacesForAddition(const SedBase * sb);


  /**
   * Returns @c true if this object's set of XML namespaces are a subset
   * of the given object's XML namespaces.
   *
   * @param sb an object to compare with respect to namespaces
   *
   * @return boolean, @c true if this object's collection of namespaces is
   * a subset of @p sb's, @c false otherwise.
   */
  bool matchesRequiredSedNamespacesForAddition(const SedBase * sb) const;


  /**
   * Sets the user data of this element. This can be used by the application
   * developer to attach custom information to the node. In case of a deep
   * copy this attribute will passed as it is. The attribute will be never
   * interpreted by this class.
   *
   * @param userData specifies the new user data.
   *
   * @return integer value indicating success/failure of the
   * function.  The possible values returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_FAILED LIBSEDML_OPERATION_FAILED @endlink
   */
  int setUserData(void *userData);


  /**
   * Returns the user data that has been previously set via setUserData().
   *
   * @return the user data of this node, or @c NULL if no user data has been set.
   *
   * @if clike
   * @see ASTNode::setUserData(void *userData)
   * @endif@~
   */
  void *getUserData() const;

  /**
   * @return the SedErrorLog used to log errors during while reading and
   * validating Sed.
   */
  virtual SedErrorLog* getErrorLog();


protected:


  /**
   * When overridden allows SedBase elements to use the text included in between
   * the elements tags. The default implementation does nothing.
   *
   * @param text the text string found between the element tags.
   */
  virtual void setElementText(const std::string &text);


  /** @cond doxygen-libsbml-internal */

  bool matchesCoreSedNamespace(const SedBase * sb);

  bool matchesCoreSedNamespace(const SedBase * sb) const;

  /**
   * Creates a new SedBase object with the given Sed level, version.
   */
  SedBase(unsigned int level, unsigned int version);


  /**
   * Creates a new SedBase object with the given SedNamespaces.
   * Only subclasses may create SedBase objects.
   */
  SedBase(SedNamespaces* sbmlns);


  /**
  * Copy constructor. Creates a copy of this SedBase object.
   *
   * @param orig the object to copy.
   *
   * @throws @if python ValueError @else SedConstructorException @endif@~
   * Thrown if the argument @p orig is @c NULL.
  */
  SedBase(const SedBase& orig);


  /**
   * Subclasses should override this method to create, store, and then
   * return an Sed object corresponding to the next XMLToken in the
   * XMLInputStream.
   *
   * @return the Sed object corresponding to next XMLToken in the
   * XMLInputStream or @c NULL if the token was not recognized.
   */
  virtual SedBase* createObject(XMLInputStream& stream);


  /**
   * Predicate returning @c true if this
   * object's level/version and namespace values correspond to a valid
   * Sed specification.
   *
   * @param typecode the typecode for this element
   * @param xmlns the namespaces used by this element.
   *
   * @note  This function is provided as convenience method to be called from constructors. This
   *        allows to use it in scenarios where the namespaces or typecode have not yet been initialized.
   *
   * @return @c true if the level, version and namespace values of this
   * Sed object correspond to a valid set of values, @c false otherwise.
   */
  bool hasValidLevelVersionNamespaceCombination(int typecode, XMLNamespaces *xmlns);


  /**
   * Subclasses should override this method to read (and store) XHTML,
   * MathML, etc. directly from the XMLInputStream.
   *
   * @return true if the subclass read from the stream, false otherwise.
   */
  virtual bool readOtherXML(XMLInputStream& stream);


  /**
   * The Sed XML Schema is written such that the order of child elements
   * is significant.  LibSed can read elements out of order.  If you
   * override this method to indicate the ordinal position of element with
   * respect to its siblings, libSed will log an error if the element is
   * read out of order.
   *
   * @return the ordinal position of the element with respect to its
   * siblings or @c -1 (the default) to indicate the position is not
   * significant.
   */
  virtual int getElementPosition() const;



  /**
   * Convenience method for easily logging problems from within method
   * implementations.
   *
   * This is essentially a short form of getErrorLog()->logError(...)
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  void logError(unsigned int       id
                , const unsigned int level   = 2
                    , const unsigned int version = 3
                        , const std::string& details = "");


  /**
   * Helper to log a common type of error.
   */
  void logUnknownAttribute(const std::string& attribute,
                           const unsigned int level,
                           const unsigned int version,
                           const std::string& element,
                           const std::string& prefix = "");


  /**
   * Helper to log a common type of error.
   */
  void logUnknownElement(const std::string& element,
                         const unsigned int level,
                         const unsigned int version);


  /**
   * Helper to log a common type of error.
   */
  void logEmptyString(const std::string& attribute,
                      const unsigned int level,
                      const unsigned int version,
                      const std::string& element);


  /**
   * Subclasses should override this method to add the list of
   * expected attributes. Be sure to call your parents implementation
   * of this method as well.
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
   *
   * (NOTICE) this function doesn't write xmlns attributes.
   * Be sure to implement wirteXMLNS() function to write xmlns attributes.
   *
   */
  virtual void writeAttributes(XMLOutputStream& stream) const;


  /**
   *
   * Subclasses should override this method to write their xmlns attriubutes
   * (if any) to the XMLOutputStream.
   *
   */
  virtual void writeXMLNS(XMLOutputStream& stream) const;


  /**
   * Synchronizes the annotation of this Sed object.
   *
   * Annotation element (XMLNode* mAnnotation) is synchronized with the
   * current CVTerm objects (List* mCVTerm).
   * Currently, this method is called in getAnnotation, isSetAnnotation,
   * and writeElements methods.
   */
  virtual void syncAnnotation();


  /**
   * Checks that the Sed element appears in the expected order.
   *
   * If @p object is not in the expected position, an error is logged.
   */
  void checkOrderAndLogError(SedBase* object, int expected);


  /**
   * Checks that an Sed ListOf element is populated.
   * If a listOf element has been declared with no elements,
   * an error is logged.
   */
  void checkListOfPopulated(SedBase* object);

  /**
   * Checks that the given default namespace in the given element is valid.
   * If the given default namespace is not valid, an error is logged.
   */
  void checkDefaultNamespace(const XMLNamespaces* xmlns,
                             const std::string& elementName, const std::string& prefix = "");

  /**
   * Checks the annotation does not declare an sbml namespace.
   * If the annotation declares an sbml namespace an error is logged.
   */
  void checkAnnotation();


  /**
   * Checks that the XHTML is valid.
   * If the xhtml does not conform to the specification of valid xhtml within
   * an sbml document, an error is logged.
   */
  void checkXHTML(const XMLNode *);


  /**
   * Sets the XML namespace to which this element belongs to.
   * For example, all elements that belong to Sed Level 1 Version 1 Core
   * must set the namespace to "http://sed-ml.org/";
   *
   * @return integer value indicating success/failure of the
   * function.  @if clike The value is drawn from the
   * enumeration #OperationReturnValues_t. @endif@~ The possible values
   * returned by this function are:
   * @li @link OperationReturnValues_t#LIBSEDML_OPERATION_SUCCESS LIBSEDML_OPERATION_SUCCESS @endlink
   * @li @link OperationReturnValues_t#LIBSEDML_INVALID_ATTRIBUTE_VALUE LIBSEDML_INVALID_ATTRIBUTE_VALUE @endlink
   */
  int setElementNamespace(const std::string &uri);


  /**
   * Gets the XML namespace (URI) to which this element belongs to.
   */
  const std::string& getElementNamespace() const;

  /**
   * Gets the URI to which this element belongs to.
   * For example, all elements that belong to Sed Level 1 Version 1 Core
   * must would have the URI "http://sed-ml.org/";
   *
   * Unlike getElementNamespace, this function first returns the URI for this
   * element by looking into the SedNamespaces object of the document with
   * the its package name. if not found it will return the result of
   * getElementNamespace
   *
   * @return the URI this elements
   *
   * @see getPackageName
   * @see getElementNamespace
   * @see getSedNamespaces
   * @see getSedDocument
   */
  std::string getURI() const;

  /**
   * Return the prefix of this element.
   */
  std::string getPrefix() const;

  /**
   * Return the Sed prefix of this element. This will be the same as getPrefix()
   * unless the element in question is an element of an Sed extension class.
   */
  std::string getSedPrefix() const;



  /**
   * Returns the root element of the node tree to which this element is connected.
   *
   * @note The root element may not be an SedDocument element. For example,
   * this element is the root element if this element doesn't have a parent
   * Sed object (i.e. mParentSedObject is NULL)
   *
   * @see enablePackageInternal
   */
  SedBase* getRootElement();


  // ------------------------------------------------------------------


  std::string     mMetaId;
  XMLNode*        mNotes;
  XMLNode*        mAnnotation;
  SedDocument*   mSed;
  SedNamespaces* mSedNamespaces;
  void*           mUserData;

  unsigned int mLine;
  unsigned int mColumn;

  /* store the parent Sed object */
  SedBase* mParentSedObject;

  /* flag that allows object to know its been deleted
   * for OS where the memory is still readable after a delete
   */
  bool mHasBeenDeleted;

  std::string mEmptyString;

  //
  // namespace to which this SedBase object belongs.
  // This variable can be publicly accessible by getElementNamespace function.
  //
  // For example, mURI of elements defined in L3 Core (or defined in Level 2
  // or before) will be the URI defined in the corresponding Sed specification
  // (e.g. "http://www.sbml.org/sbml/level3/version1" for L3V1 Core); and mURI
  // will be "http://www.sbml.org/sbml/level3/version1/layout/version1" for
  // elements defined in layout extension L3V1-V1.
  // The purpuse of this data member is to explicitly distinguish elements in core
  // package and extension packages.
  //
  // From the implementation point of view, this variable is needed to identify
  // if the prefix needs to be added when printing elements in some package extension.
  // (i.e. used in getPrefix function)
  //
  std::string mURI;

  bool getHasBeenDeleted() const;

  /** @endcond */

private:
  /** @cond doxygen-libsbml-internal */

  /**
   * Stores the location (line and column) and any XML namespaces (for
   * roundtripping) declared on this Sed (XML) element.
   */
  void setSedBaseFields(const XMLToken& element);


  /**
   * @return true if read an <annotation> element from the stream
   */
  bool readAnnotation(XMLInputStream& stream);


  /**
   * @return true if read a <notes> element from the stream
   */
  bool readNotes(XMLInputStream& stream);


  /** @endcond */
};

LIBSEDML_CPP_NAMESPACE_END

#endif  /* __cplusplus */



#ifndef SWIG

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/* ----------------------------------------------------------------------------
 * See the .cpp file for the documentation of the following functions.
 * --------------------------------------------------------------------------*/


LIBSEDML_EXTERN
const char *
SedBase_getMetaId(SedBase_t *sb);




LIBSEDML_EXTERN
const SedDocument_t *
SedBase_getSedDocument(SedBase_t *sb);


LIBSEDML_EXTERN
const SedBase_t *
SedBase_getParentSedObject(SedBase_t *sb);


LIBSEDML_EXTERN
const SedBase_t *
SedBase_getAncestorOfType(SedBase_t *sb, int type, const char* pkgName);


LIBSEDML_EXTERN
unsigned int
SedBase_getLine(const SedBase_t *sb);


LIBSEDML_EXTERN
unsigned int
SedBase_getColumn(const SedBase_t *sb);


LIBSEDML_EXTERN
XMLNode_t *
SedBase_getNotes(SedBase_t *sb);


LIBSEDML_EXTERN
char*
SedBase_getNotesString(SedBase_t *sb);


LIBSEDML_EXTERN
XMLNode_t *
SedBase_getAnnotation(SedBase_t *sb);


LIBSEDML_EXTERN
char*
SedBase_getAnnotationString(SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_isSetMetaId(const SedBase_t *sb);




LIBSEDML_EXTERN
int
SedBase_isSetNotes(const SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_isSetAnnotation(const SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_setMetaId(SedBase_t *sb, const char *metaid);


LIBSEDML_EXTERN
int
SedBase_setNamespaces(SedBase_t *sb, XMLNamespaces_t *xmlns);


LIBSEDML_EXTERN
int
SedBase_setNotes(SedBase_t *sb, XMLNode_t *notes);


LIBSEDML_EXTERN
int
SedBase_setNotesString(SedBase_t *sb, char *notes);


LIBSEDML_EXTERN
int
SedBase_setNotesStringAddMarkup(SedBase_t *sb, char *notes);


LIBSEDML_EXTERN
int
SedBase_appendNotes(SedBase_t *sb, XMLNode_t *notes);


LIBSEDML_EXTERN
int
SedBase_appendNotesString(SedBase_t *sb, char *notes);


LIBSEDML_EXTERN
int
SedBase_setAnnotation(SedBase_t *sb, XMLNode_t *annotation);


LIBSEDML_EXTERN
int
SedBase_setAnnotationString(SedBase_t *sb, char *annotation);


LIBSEDML_EXTERN
int
SedBase_appendAnnotation(SedBase_t *sb, XMLNode_t *annotation);


LIBSEDML_EXTERN
int
SedBase_appendAnnotationString(SedBase_t *sb, char *annotation);


LIBSEDML_EXTERN
int
SedBase_appendAnnotationString(SedBase_t *sb, char *annotation);


LIBSEDML_EXTERN
int
SedBase_removeTopLevelAnnotationElement(SedBase_t *sb, char *name);


LIBSEDML_EXTERN
int
SedBase_removeTopLevelAnnotationElementWithURI(SedBase_t *sb, const char *name,
    const char *uri);


LIBSEDML_EXTERN
int
SedBase_replaceTopLevelAnnotationElement(SedBase_t *sb, XMLNode_t *annotation);


LIBSEDML_EXTERN
int
SedBase_replaceTopLevelAnnotationElementString(SedBase_t *sb, char *annotation);


LIBSEDML_EXTERN
int
SedBase_unsetMetaId(SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_unsetNotes(SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_unsetAnnotation(SedBase_t *sb);


LIBSEDML_EXTERN
const Model_t *
SedBase_getModel(const SedBase_t *sb);


LIBSEDML_EXTERN
unsigned int
SedBase_getLevel(const SedBase_t *sb);


LIBSEDML_EXTERN
unsigned int
SedBase_getVersion(const SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_getTypeCode(const SedBase_t *sb);


LIBSEDML_EXTERN
const char *
SedBase_getElementName(const SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_hasValidLevelVersionNamespaceCombination(SedBase_t *sb);


LIBSEDML_EXTERN
int
SedBase_setUserData(SedBase_t* sb, void *userData);

LIBSEDML_EXTERN
void *
SedBase_getUserData(SedBase_t* sb);

LIBSEDML_EXTERN
SedBase_t*
SedBase_getElementBySId(SedBase_t* sb, const char* id);

LIBSEDML_EXTERN
SedBase_t*
SedBase_getElementByMetaId(SedBase_t* sb, const char* metaid);

LIBSEDML_EXTERN
List_t*
SedBase_getAllElements(SedBase_t* sb);

LIBSEDML_EXTERN
void
SedBase_renameSIdRefs(SedBase_t* sb, const char* oldid, const char* newid);


END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /* !SWIG   */
#endif  /* SedBase_h */
