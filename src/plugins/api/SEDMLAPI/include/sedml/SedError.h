/**
 * @file    SedError.h
 * @brief   Represents Sed errors and other diagnostics
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
 * @class SedError
 * @ingroup Core
 * @brief Representation of errors, warnings and other diagnostics
 *
 * @htmlinclude not-sbml-warning.html
 *
 * When a libSed operation on Sed content results in an error, or when
 * there is something wrong with the Sed content, the problems are
 * reported as SedError objects.  These are generally stored in an
 * SedErrorLog object; this log object, in turn, is kept in the
 * SedDocument object containing the Sed content.  Applications can
 * obtain the list of logged errors using SedDocument::getErrorLog() and
 * then use the methods provided by SedErrorLog to access individual
 * SedError objects.  (Note that despite the word "error" in the name,
 * SedError objects are used to represent not only "true" errors, but also
 * warnings and some informational diagnostics.  The name is a historical
 * hold-over from early versions of libSed, in which the object really was
 * only used to report errors.)
 *
 * @if clike
 * Each SedError object instance has an identification number that
 * identifies the nature of the problem.  This "error id" number will be up
 * to five digits long, and it will be listed in one of two enumerations:
 * <a class="el" href="#SedErrorCode_t"> SedErrorCode_t</a> (described <a
 * class="el" href="#SedErrorCode_t"> below</a>) or @link
 * XMLError::XMLErrorCode_t XMLErrorCode_t @endlink (described in the
 * documentation for the class XMLError).  The former enumeration contains
 * all the Sed validation rule numbers listed in the appendices of the
 * Sed specification documents, as well as some additional
 * libSed-specific error codes.
 * @endif@if java
 * Each SedError object instance has an identification number that
 * identifies the nature of the problem.  This "error id" number will be up
 * to five digits long, and it will come from one of two sets of static
 * integer constants defined in the interface class <code><a
 * href="libsbmlConstants.html"> libsbmlConstants</a></code>: either the
 * Sed error identifiers <a class="el" href="#SedErrorCode_t"> (described
 * below)</a> or the XML error identifiers (described in the documentation
 * for the class <code><a href="XMLError.html"> XMLError</a></code>).  The
 * former set of constants includes all the Sed validation rule numbers
 * listed in the appendices of the Sed specification documents, as well as
 * some additional libSed-specific error codes.
 * @endif@if python
 * Each SedError object instance has an identification number that
 * identifies the nature of the problem.  This "error id" number will be up
 * to five digits long, and it will come from one
 * of two sets of static integer constants defined in
 * the interface class @link libsbml libsbml@endlink: either the Sed
 * error identifiers <a
 * class="el" href="#SedErrorCode_t"> (described below)</a> or the XML
 * error identifiers (described in the documentation for the class XMLError).
 * The former set of constants
 * includes all the Sed validation rule numbers listed in the appendices
 * of the Sed specification documents, as well as some additional
 * libSed-specific error codes.
 * @endif@~
 *
 * Error codes are useful mainly for software.  For human readers,
 * SedError also includes text messages that describe the nature of a
 * given problem.  The messages can be accessed using
 * SedError::getShortMessage() and SedError::getMessage().  The former
 * provides a brief one-line description of the issue, while
 * SedError::getMessage() provides a more detailed text, including (if
 * appropriate) references to sections of the Sed specifications where
 * relevant topics are discussed.  These text strings are suitable for
 * displaying to human users.
 *
 * @if clike
 * An SedError object also contains a category code; its value may be
 * retrieved using the method SedError::getCategory().  Category values
 * are drawn from the enumeration <a class="el"
 * href="#SedErrorCategory_t">SedErrorCategory_t</a> described below.
 * Categories are used to partition errors into distinct conceptual groups.
 * This is principally used by the libSed validation system to group
 * classes of validation checks.  For example,
 * @link SedErrorCategory_t#LIBSEDML_CAT_IDENTIFIER_CONSISTENCY LIBSEDML_CAT_IDENTIFIER_CONSISTENCY@endlink
 * is the category for tests that check identifier consistency;
 * @link SedErrorCategory_t#LIBSEDML_CAT_MATHML_CONSISTENCY LIBSEDML_CAT_MATHML_CONSISTENCY@endlink
 * is the category for MathML consistency checking; and
 * so on.
 * @endif@if java
 * An SedError object also contains a category code; its value may be
 * retrieved using the method SedError::getCategory().  Category values
 * are drawn from a set of static integer constants
 * defined in <code><a href="libsbmlConstants.html">libsbmlConstants</a></code>,
 * and having names beginning with the characters
 * <code>LIBSEDML_CAT_</code>.  The list of possible codes is described in a
 * separate section below.  Categories are used to partition errors into
 * distinct conceptual groups.  This is principally used by the libSed
 * validation system to group classes of validation checks.  For example,
 * @link SedErrorCategory_t#LIBSEDML_CAT_IDENTIFIER_CONSISTENCY LIBSEDML_CAT_IDENTIFIER_CONSISTENCY@endlink
 * is the category for tests that check identifier consistency;
 * @link SedErrorCategory_t#LIBSEDML_CAT_MATHML_CONSISTENCY LIBSEDML_CAT_MATHML_CONSISTENCY@endlink
 * is the category for MathML consistency checking; and
 * so on.
 * @endif@if python
 * An SedError object also contains a category code; its value may be
 * retrieved using the method SedError::getCategory().  Category values
 * are drawn from a set of static integer constants
 * defined in @link libsbml libsbml@endlink and having names beginning with the characters
 * <code>LIBSEDML_CAT_</code>.  The list of possible codes is described in a
 * separate section below.  Categories are used to partition errors into
 * distinct conceptual groups.  This is principally used by the libSed
 * validation system to group classes of validation checks.  For example,
 * @link SedErrorCategory_t#LIBSEDML_CAT_IDENTIFIER_CONSISTENCY LIBSEDML_CAT_IDENTIFIER_CONSISTENCY@endlink
 * is the category for tests that check identifier consistency;
 * @link SedErrorCategory_t#LIBSEDML_CAT_MATHML_CONSISTENCY LIBSEDML_CAT_MATHML_CONSISTENCY@endlink
 * is the category for MathML consistency checking; and
 * so on.
 * @endif@~
 *
 * In addition, SedError also has a severity code.  Its value may be
 * retrieved using the method SedError::getSeverity().  The possible
 * severity values are the same as those reported by @if clike XMLError.@endif@if python XMLError.@endif@if java <code><a href="XMLError.html">XMLError</a></code>.@endif@~
 * Severity levels currently range from informational
 * (@link XMLErrorSeverity_t#LIBSEDML_SEV_INFO LIBSEDML_SEV_INFO@endlink)
 * to fatal errors
 * (@link XMLErrorSeverity_t#LIBSEDML_SEV_FATAL LIBSEDML_SEV_FATAL@endlink).
 * They can be
 * used by an application to evaluate how serious a given problem
 * is.
 *
 * Finally, SedError records the line and column near where the problem
 * occurred in the Sed content.  The values may be retrieved using the
 * methods SedError::getLine() and SedError::getColumn().  We say "near",
 * because a lot of factors affect how accurate the line/column information
 * ultimately is.  For example, different XML parsers have different
 * conventions for which line and column number they report for a
 * particular problem (which makes a difference when a problem involves an
 * opening XML tag on one line and a closing tag on another line).  In some
 * situations, some parsers report invalid line and/or column numbers
 * altogether.  If this occurs, libSed sets the line and/or column number
 * in the SedError object to the the value of the maximum unsigned long
 * integer representable on the platform where libSed is running.  (This
 * is equal to the constant named <code>ULONG_MAX</code> in C and C++.)
 * The probability that a true line or column number in an Sed model would
 * equal this value is vanishingly small; thus, if an application
 * encounters these values in an XMLError object, it can assume no valid
 * line/column number could be provided by libSed in that situation.
 *
 * @if clike
 * <h3><a class="anchor" name="SedErrorCode_t">SedErrorCode_t</a></h3>
 *
 * SedErrorCode_t is an enumeration of all Sed-level error, warning and
 * informational diagnostic codes.  Every SedError object has an error
 * code value that can be either a value from this enumeration, or a value
 * from the @link XMLError::XMLErrorCode_t XMLErrorCode_t @endlink
 * enumeration (see the documentation for XMLError).  The latter values
 * apply when the error or warning signifies a basic XML issue rather than
 * an Sed issue per se.  The values of SedErrorCode_t are distinguished
 * from those of @link XMLError::XMLErrorCode_t XMLErrorCode_t @endlink by
 * being numbered 10000 and higher, while the XML layer's codes are 9999 and
 * lower.  The method SedError::getErrorId() returns the error code of a
 * given SedError object instance.
 *
 * The following is a table of the symbolic names of SedErrorCode_t values
 * and the meaning of each code.  In this table, the right-hand columns
 * titled "L1V1", "L1V2", etc. refer to Levels and Versions of the Sed
 * specifications, and the entries in each column refer to whether the
 * severity of the condition in that particular Level+Version of Sed.
 * The codes stand for the following:
 *
 * @endif@if java <h3><a class="anchor"
 * name="SedErrorCode_t">Error codes associated with SedError objects</a></h3>
 *
 * The error and warning codes returned by libSed are listed in the table
 * below.  The method SedError::getErrorId() returns the error code of a
 * given SedError object instance.  In the libSed Java language
 * interface, these error identifiers are currently
 * implemented as static integer constants defined in the interface class
 * <code><a href="libsbmlConstants.html">libsbmlConstants</a></code>.  This
 * is admittedly not an ideal approach from the standpoint of modern Java
 * programming, but it was necessary to work around the lack of
 * enumerations in Java prior to JDK 1.5.  Future versions of libSed may
 * use a proper Java enumeration type to define the error identifiers.
 *
 * In this table, the right-hand columns titled "L1V1", "L1V2", etc. refer
 * to Levels and Versions of the Sed specifications, and the entries in
 * each column refer to whether the severity of the condition in that
 * particular Level+Version of Sed.  The codes stand for the following:
 *
 * @endif@if python <h3><a class="anchor"
 * name="SedErrorCode_t">Error codes associated with SedError objects</a></h3>
 *
 * The error and warning codes returned by libSed are listed in the table
 * below.  The method SedError::getErrorId() returns the error code of a
 * given SedError object instance.  In the libSed Python language
 * interface, these error identifiers are currently
 * implemented as static integer constants defined in the interface class
 * @link libsbml libsbml@endlink.
 *
 * In this table, the right-hand columns titled "L1V1", "L1V2", etc. refer
 * to Levels and Versions of the Sed specifications, and the entries in
 * each column refer to whether the severity of the condition in that
 * particular Level+Version of Sed.  The codes stand for the following:
 *
 * @endif@~
 *
 * <table cellspacing="1" cellpadding="2" border="0" class="normal-font">
 * <tr><td class="s-na">N</td><td>= Not applicable</td></tr>
 * <tr><td class="s-info">I</td><td>= Informational</td></tr>
 * <tr><td class="s-warning">W</td><td>= Warning</td></tr>
 * <tr><td class="s-error">E</td><td>= Error</td></tr>
 * <tr><td class="s-fatal">F</td><td>= Fatal</td></tr>
 * </table>
 *
 * The text shown in the "Meaning" is the text returned by the
 * SedError::getShortMessage() method on a given SedError object.  A
 * longer and (hopefully) clearer explanation of the issue is returned by
 * SedError::getMessage().
 *
 * @htmlinclude sbmlerror-table.html
 *
 * @if clike <h3><a class="anchor" name="SedErrorCategory_t">SedErrorCategory_t</a></h3>
 *
 * SedErrorCategory_t is an enumeration of category codes for SedError
 * diagnostics.  The category can be retrieved from an SedError object
 * using the method SedError::getCategory().  These enumeration values are
 * distinct from (and in addition to) the @link
 * XMLError::XMLErrorCategory_t XMLErrorCategory_t @endlink codes used by
 * the parent XMLError object.  User programs receiving an SedError object
 * can use this distinction to check whether the error represents a
 * low-level XML problem or an Sed problem.
 *
 * The following table lists each possible value and a brief description of
 * its meaning.
 *
 * @endif@if python <h3><a class="anchor" name="SedErrorCategory_t">Category codes associated with SedError objects</a></h3>
 *
 * As discussed above, each SedError object contains a value for a
 * category identifier, describing the type of issue that the SedError
 * object represents.  The category can be retrieved from an SedError
 * object using the method SedError::getCategory().  The following table
 * lists each possible value and a brief description of its meaning.
 *
 * As is the case with the error codes, in the libSed Python language
 * interface, the category identifiers are currently implemented as static
 * integer constants defined in the interface class
 * @link libsbml libsbml@endlink.
 *
 * The following table lists each possible value and a brief description of
 * its meaning.
 *
 * @endif@if java <h3><a class="anchor"
 * name="SedErrorCategory_t">Category codes associated with SedError objects</a></h3>
 *
 * As discussed above, each SedError object contains a value for a
 * category identifier, describing the type of issue that the SedError
 * object represents.  The category can be retrieved from an SedError
 * object using the method SedError::getCategory().  The following table
 * lists each possible value and a brief description of its meaning.
 *
 * As is the case with the error codes, in the libSed Java language
 * interface, the category identifiers are currently implemented as static
 * integer constants defined in the interface class
 * {@link libsbmlConstants}.
 *
 * The following table lists each possible value and a brief description of
 * its meaning.
 *
 * @endif@if csharp <h3><a class="anchor"
 * name="SedErrorCategory_t">Category codes associated with SedError objects</a></h3>
 *
 * As discussed above, each SedError object contains a value for a
 * category identifier, describing the type of issue that the SedError
 * object represents.  The category can be retrieved from an SedError
 * object using the method SedError::getCategory().  The following table
 * lists each possible value and a brief description of its meaning.
 *
 * As is the case with the error codes, in the libSed C# language
 * interface, the category identifiers are currently implemented as static
 * integer constants defined in the interface class
 * {@link libsbmlcs.libsbml}.
 *
 * The following table lists each possible value and a brief description of
 * its meaning.
 *
 * @endif@~
 *
 * <center>
 * <table width="90%" cellspacing="1" cellpadding="4" border="0"  class="text-table normal-font alt-row-colors">
 *  <tr style="background: lightgray" class="normal-font">
 *      <th>Enumerator</td>
 *      <th>Meaning</td>
 *  </tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML LIBSEDML_CAT_SEDML@endlink</td><td>General error not falling into
 * another category below.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L1_COMPAT LIBSEDML_CAT_SEDML_L1_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model from Sed Level&nbsp;2 to Sed
 * Level&nbsp;1.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L2V1_COMPAT LIBSEDML_CAT_SEDML_L2V1_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model to Sed Level&nbsp;2
 * Version&nbsp;1.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L2V2_COMPAT LIBSEDML_CAT_SEDML_L2V2_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model to Sed Level&nbsp;2
 * Version&nbsp;2.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_GENERAL_CONSISTENCY LIBSEDML_CAT_GENERAL_CONSISTENCY@endlink</td><td>Category of
 * errors that can occur while validating general Sed constructs.  With
 * respect to the Sed specification, these concern failures in applying
 * the validation rules numbered 2xxxx in the Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_IDENTIFIER_CONSISTENCY LIBSEDML_CAT_IDENTIFIER_CONSISTENCY@endlink</td><td>Category of
 * errors that can occur while validating symbol identifiers in a model.
 * With respect to the Sed specification, these concern failures in
 * applying the validation rules numbered 103xx in the Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_UNITS_CONSISTENCY LIBSEDML_CAT_UNITS_CONSISTENCY@endlink</td><td>Category of
 * errors that can occur while validating the units of measurement on
 * quantities in a model.  With respect to the Sed specification, these
 * concern failures in applying the validation rules numbered 105xx in the
 * Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_MATHML_CONSISTENCY LIBSEDML_CAT_MATHML_CONSISTENCY@endlink</td><td>Category of
 * errors that can occur while validating MathML formulas in a model.  With
 * respect to the Sed specification, these concern failures in applying
 * the validation rules numbered 102xx in the Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SBO_CONSISTENCY LIBSEDML_CAT_SBO_CONSISTENCY@endlink</td><td>Category of errors
 * that can occur while validating SBO identifiers in a model.  With
 * respect to the Sed specification, these concern failures in applying
 * the validation rules numbered 107xx in the Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_OVERDETERMINED_MODEL LIBSEDML_CAT_OVERDETERMINED_MODEL@endlink</td><td>Error in the
 * system of equations in the model: the system is overdetermined,
 * therefore violating a tenet of proper Sed.  With respect to the Sed
 * specification, this is validation rule #10601 in the Sed Level&nbsp;2 Versions&nbsp;2&ndash;4
 * and Level&nbsp;3 Version&nbsp;1 specifications.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L2V3_COMPAT LIBSEDML_CAT_SEDML_L2V3_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model to Sed Level&nbsp;2
 * Version&nbsp;3.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_MODELING_PRACTICE LIBSEDML_CAT_MODELING_PRACTICE@endlink</td><td>Category of
 * warnings about recommended good practices involving Sed and
 * computational modeling.  (These are tests performed by libSed and do
 * not have equivalent Sed validation rules.)</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_INTERNAL_CONSISTENCY LIBSEDML_CAT_INTERNAL_CONSISTENCY@endlink</td><td>Category of
 * errors that can occur while validating libSed's internal representation
 * of Sed constructs. (These are tests performed by libSed and do
 * not have equivalent Sed validation rules.)</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L2V4_COMPAT LIBSEDML_CAT_SEDML_L2V4_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model to Sed Level&nbsp;2
 * Version&nbsp;4.</td></tr>
 * <tr><td>@link XMLErrorCategory_t#LIBSEDML_CAT_SEDML_L3V1_COMPAT LIBSEDML_CAT_SEDML_L3V1_COMPAT@endlink</td><td>Category of errors
 * that can only occur during attempted translation from one Level/Version
 * of Sed to another.  This particular category applies to errors
 * encountered while trying to convert a model to Sed Level&nbsp;3
 * Version&nbsp;1.</td></tr>
 *
 * </table>
 * </center>
 *
 * @if clike
 * <h3><a class="anchor" name="SedErrorSeverity_t">SedErrorSeverity_t</a></h3>
 *
 * This is an enumeration of severity codes for SedError diagnostics.
 * User programs receiving an SedError object can use this distinction to
 * check whether the error represents a low-level XML problem or an Sed
 * problem.
 *
 * In libSed version @htmlinclude libsbml-version.html
 * there are no additional severity codes in SedErrorSeverity_t beyond
 * those defined in @link XMLError::XMLErrorSeverity_t XMLErrorSeverity_t@endlink.
 *
 * <hr>
 * @endif@if java <h3><a class="anchor"
 * name="SedErrorSeverity_t">Severity codes associated with SedError
 * objects</h3>
 *
 * In libSed version @htmlinclude libsbml-version.html
 * there are no additional severity codes beyond those defined by XMLError.
 * They are implemented as static integer constants defined in the interface
 * class <code><a href="libsbmlConstants.html">libsbmlConstants</a></code>,
 * and have names beginning with <code>LIBSEDML_SEV_</code>.
 * @endif@if python <h3><a class="anchor"
 * name="SedErrorSeverity_t">Severity codes associated with SedError
 * objects</h3>
 *
 * In libSed version @htmlinclude libsbml-version.html
 * there are no additional severity codes beyond those defined by XMLError.
 * They are implemented as static integer constants defined in the
 * interface class @link libsbml libsbml@endlink, and have names beginning
 * with <code>LIBSEDML_SEV_</code>.
 * @endif@~
 */

#ifndef SedError_h
#define SedError_h

#include <sedml/common/extern.h>
#include <sbml/xml/XMLError.h>
#include <sedml/SedNamespaces.h>


LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS

/**
 * Codes for all Sed-level errors and warnings.
 *
 * These are distinguished from the XML layer (LIBLAX) error codes by being
 * numbered > 10000, while the XML layer's codes are < 9999.  Calling
 * programs may wish to check whether a given SedError object's error
 * identifier is actually from SedErrorCode_t or XMLError::XMLErrorCode_t.
 * This distinction corresponds to whether a given error represents a
 * low-level XML problem or an Sed problem.
 */
typedef enum
{
  SedUnknownError                          = 10000 /*!< Encountered unknown internal libSed error. */
  , SedNotUTF8                               = 10101 /*!< File does not use UTF-8 encoding. */
  , SedUnrecognizedElement                   = 10102 /*!< Encountered unrecognized element. */
  , SedNotSchemaConformant                   = 10103 /*!< Document does not conform to the Sed XML schema. */
  , SedL3NotSchemaConformant                 = 10104 /*!< Document is not well-formed XML. */
  , SedInvalidMathElement                    = 10201 /*!< Invalid MathML. */
  , SedDisallowedMathMLSymbol                = 10202 /*!< Disallowed MathML symbol found. */
  , SedDisallowedMathMLEncodingUse           = 10203 /*!< Use of the MathML 'encoding' attribute is not allowed on this element. */
  , SedDisallowedDefinitionURLUse            = 10204 /*!< Use of the MathML 'definitionURL' attribute is not allowed on this element. */
  , SedBadCsymbolDefinitionURLValue          = 10205 /*!< Invalid <code>&lt;csymbol&gt;</code> 'definitionURL' attribute value. */
  , SedDisallowedMathTypeAttributeUse        = 10206 /*!< Use of the MathML 'type' attribute is not allowed on this element. */
  , SedDisallowedMathTypeAttributeValue      = 10207 /*!< Disallowed MathML 'type' attribute value. */
  , SedLambdaOnlyAllowedInFunctionDef        = 10208 /*!< Use of <code>&lt;lambda&gt;</code> not permitted outside of FunctionDefinition objects. */
  , SedBooleanOpsNeedBooleanArgs             = 10209 /*!< Non-Boolean argument given to Boolean operator. */
  , SedNumericOpsNeedNumericArgs             = 10210 /*!< Non-numerical argument given to numerical operator. */
  , SedArgsToEqNeedSameType                  = 10211 /*!< Arguments to <code>&lt;eq&gt;</code> and <code>&lt;neq&gt;</code> must have the same data types. */
  , SedPiecewiseNeedsConsistentTypes         = 10212 /*!< Terms in a <code>&lt;piecewise&gt;</code> expression must have consistent data types. */
  , SedPieceNeedsBoolean                     = 10213 /*!< The second argument of a <code>&lt;piece&gt;</code> expression must yield a Boolean value. */
  , SedApplyCiMustBeUserFunction             = 10214 /*!< A <code>&lt;ci&gt;</code> element in this context must refer to a function definition. */
  , SedApplyCiMustBeModelComponent           = 10215 /*!< A <code>&lt;ci&gt;</code> element in this context must refer to a model component. */
  , SedKineticLawParametersAreLocalOnly      = 10216 /*!< Cannot use a KineticLaw local parameter outside of its local scope. */
  , SedMathResultMustBeNumeric               = 10217 /*!< A formula's result in this context must be a numerical value. */
  , SedOpsNeedCorrectNumberOfArgs            = 10218 /*!< Incorrect number of arguments given to MathML operator. */
  , SedInvalidNoArgsPassedToFunctionDef      = 10219 /*!< Incorrect number of arguments given to function invocation. */
  , SedDisallowedMathUnitsUse                = 10220 /*!< Attribute 'units' is only permitted on <code>&lt;cn&gt;</code> elements. */
  , SedInvalidUnitsValue                     = 10221 /*!< Invalid value given for the 'units' attribute. */
  , SedDuplicateComponentId                  = 10301 /*!< Duplicate 'id' attribute value. */
  , SedDuplicateUnitDefinitionId             = 10302 /*!< Duplicate unit definition 'id' attribute value. */
  , SedDuplicateLocalParameterId             = 10303 /*!< Duplicate local parameter 'id' attribute value. */
  , SedMultipleAssignmentOrRateRules         = 10304 /*!< Multiple rules for the same variable are not allowed. */
  , SedMultipleEventAssignmentsForId         = 10305 /*!< Multiple event assignments for the same variable are not allowed. */
  , SedEventAndAssignmentRuleForId           = 10306 /*!< An event assignment and an assignment rule must not have the same value for 'variable'. */
  , SedDuplicateMetaId                       = 10307 /*!< Duplicate 'metaid' attribute value. */
  , SedInvalidSBOTermSyntax                  = 10308 /*!< Invalid syntax for an 'sboTerm' attribute value. */
  , SedInvalidMetaidSyntax                   = 10309 /*!< Invalid syntax for a 'metaid' attribute value. */
  , SedInvalidIdSyntax                       = 10310 /*!< Invalid syntax for an 'id' attribute value. */
  , SedInvalidUnitIdSyntax                   = 10311 /*!< Invalid syntax for the identifier of a unit. */
  , SedInvalidNameSyntax                     = 10312 /*!< Invalid syntax for a 'name' attribute value. */
  , SedMissingAnnotationNamespace            = 10401 /*!< Missing declaration of the XML namespace for the annotation. */
  , SedDuplicateAnnotationNamespaces         = 10402 /*!< Multiple annotations using the same XML namespace. */
  , SedSedNamespaceInAnnotation             = 10403 /*!< The Sed XML namespace cannot be used in an Annotation object. */
  , SedMultipleAnnotations                   = 10404 /*!< Only one Annotation object is permitted under a given Sed object. */
  , SedInconsistentArgUnits                  = 10501 /*!< The units of the function call's arguments are not consistent with its definition. */
  , SedInconsistentKineticLawUnitsL3         = 10503 /*!< The kinetic law's units are inconsistent with those of other kinetic laws in the model. */
  , SedAssignRuleCompartmentMismatch         = 10511 /*!< Mismatched units in assignment rule for compartment. */
  , SedAssignRuleSpeciesMismatch             = 10512 /*!< Mismatched units in assignment rule for species. */
  , SedAssignRuleParameterMismatch           = 10513 /*!< Mismatched units in assignment rule for parameter. */
  , SedAssignRuleStoichiometryMismatch       = 10514 /*!< Mismatched units in assignment rule for stoichiometry. */
  , SedInitAssignCompartmenMismatch          = 10521 /*!< Mismatched units in initial assignment to compartment. */
  , SedInitAssignSpeciesMismatch             = 10522 /*!< Mismatched units in initial assignment to species. */
  , SedInitAssignParameterMismatch           = 10523 /*!< Mismatched units in initial assignment to parameter. */
  , SedInitAssignStoichiometryMismatch       = 10524 /*!< Mismatched units in initial assignment to stoichiometry. */
  , SedRateRuleCompartmentMismatch           = 10531 /*!< Mismatched units in rate rule for compartment. */
  , SedRateRuleSpeciesMismatch               = 10532 /*!< Mismatched units in rate rule for species. */
  , SedRateRuleParameterMismatch             = 10533 /*!< Mismatched units in rate rule for parameter. */
  , SedRateRuleStoichiometryMismatch         = 10534 /*!< Mismatched units in rate rule for stoichiometry. */
  , SedKineticLawNotSubstancePerTime         = 10541 /*!< The units of the kinetic law are not 'substance'/'time'. */
  , SedSpeciesInvalidExtentUnits             = 10542 /*!< The species' units are not consistent with units of extent. */
  , SedDelayUnitsNotTime                     = 10551 /*!< The units of the delay expression are not units of time. */
  , SedEventAssignCompartmentMismatch        = 10561 /*!< Mismatched units in event assignment for compartment. */
  , SedEventAssignSpeciesMismatch            = 10562 /*!< Mismatched units in event assignment for species. */
  , SedEventAssignParameterMismatch          = 10563 /*!< Mismatched units in event assignment for parameter. */
  , SedEventAssignStoichiometryMismatch      = 10564 /*!< Mismatched units in event assignment for stoichiometry. */
  , SedPriorityUnitsNotDimensionless         = 10565 /*!< The units of a priority expression must be 'dimensionless'. */
  , SedUpperUnitBound                        = 10599 /*!< Upper boundary of unit validation diagnostic codes. */
  , SedOverdeterminedSystem                  = 10601 /*!< The model is overdetermined. */
  , SedInvalidModelSBOTerm                   = 10701 /*!< Invalid 'sboTerm' attribute value for a Model object. */
  , SedInvalidFunctionDefSBOTerm             = 10702 /*!< Invalid 'sboTerm' attribute value for a FunctionDefinition object. */
  , SedInvalidParameterSBOTerm               = 10703 /*!< Invalid 'sboTerm' attribute value for a Parameter object. */
  , SedInvalidInitAssignSBOTerm              = 10704 /*!< Invalid 'sboTerm' attribute value for an InitialAssignment object. */
  , SedInvalidRuleSBOTerm                    = 10705 /*!< Invalid 'sboTerm' attribute value for a Rule object. */
  , SedInvalidConstraintSBOTerm              = 10706 /*!< Invalid 'sboTerm' attribute value for a Constraint object. */
  , SedInvalidReactionSBOTerm                = 10707 /*!< Invalid 'sboTerm' attribute value for a Reaction object. */
  , SedInvalidSpeciesReferenceSBOTerm        = 10708 /*!< Invalid 'sboTerm' attribute value for a SpeciesReference object. */
  , SedInvalidKineticLawSBOTerm              = 10709 /*!< Invalid 'sboTerm' attribute value for a KineticLaw object. */
  , SedInvalidEventSBOTerm                   = 10710 /*!< Invalid 'sboTerm' attribute value for an Event object. */
  , SedInvalidEventAssignmentSBOTerm         = 10711 /*!< Invalid 'sboTerm' attribute value for an EventAssignment object. */
  , SedInvalidCompartmentSBOTerm             = 10712 /*!< Invalid 'sboTerm' attribute value for a Compartment object. */
  , SedInvalidSpeciesSBOTerm                 = 10713 /*!< Invalid 'sboTerm' attribute value for a Species object. */
  , SedInvalidCompartmentTypeSBOTerm         = 10714 /*!< Invalid 'sboTerm' attribute value for a CompartmentType object. */
  , SedInvalidSpeciesTypeSBOTerm             = 10715 /*!< Invalid 'sboTerm' attribute value for a SpeciesType object. */
  , SedInvalidTriggerSBOTerm                 = 10716 /*!< Invalid 'sboTerm' attribute value for an Event Trigger object. */
  , SedInvalidDelaySBOTerm                   = 10717 /*!< Invalid 'sboTerm' attribute value for an Event Delay object. */
  , SedNotesNotInXHTMLNamespace              = 10801 /*!< Notes must be placed in the XHTML XML namespace. */
  , SedNotesContainsXMLDecl                  = 10802 /*!< XML declarations are not permitted in Notes objects. */
  , SedNotesContainsDOCTYPE                  = 10803 /*!< XML DOCTYPE elements are not permitted in Notes objects. */
  , SedInvalidNotesContent                   = 10804 /*!< Invalid notes content found. */
  , SedOnlyOneNotesElementAllowed            = 10805 /*!< Only one Notes subobject is permitted on a given Sed object. */
  , SedInvalidNamespaceOnSed                = 20101 /*!< Invalid XML namespace for the Sed container element. */
  , SedMissingOrInconsistentLevel            = 20102 /*!< Missing or inconsistent value for the 'level' attribute. */
  , SedMissingOrInconsistentVersion          = 20103 /*!< Missing or inconsistent value for the 'version' attribute. */
  , SedPackageNSMustMatch                    = 20104 /*!< Inconsistent or invalid Sed Level/Version for the package namespace declaration. */
  , SedLevelPositiveInteger                  = 20105 /*!< The 'level' attribute must have a positive integer value. */
  , SedVersionPositiveInteger                = 20106 /*!< The 'version' attribute must have a positive integer value. */
  , SedAllowedAttributesOnSed               = 20108 /*!< Invalid attribute found on the Sed container element. */
  , SedL3PackageOnLowerSed                  = 20109 /*!< An L3 package ns found on the Sed container element.. */
  , SedMissingModel                          = 20201 /*!< No model definition found. */
  , SedIncorrectOrderInModel                 = 20202 /*!< Incorrect ordering of components within the Model object. */
  , SedEmptyListElement                      = 20203 /*!< Empty ListOf___ object found. */
  , SedNeedCompartmentIfHaveSpecies          = 20204 /*!< The presence of a species requires a compartment. */
  , SedOneOfEachListOf                       = 20205 /*!< Only one of each kind of ListOf___ object is allowed inside a Model object. */
  , SedOnlyFuncDefsInListOfFuncDefs          = 20206 /*!< Only FunctionDefinition, Notes and Annotation objects are allowed in ListOfFunctionDefinitions. */
  , SedOnlyUnitDefsInListOfUnitDefs          = 20207 /*!< Only UnitDefinition, Notes and Annotation objects are allowed in ListOfUnitDefinitions objects. */
  , SedOnlyCompartmentsInListOfCompartments  = 20208 /*!< Only Compartment, Notes and Annotation objects are allowed in ListOfCompartments objects. */
  , SedOnlySpeciesInListOfSpecies            = 20209 /*!< Only Species, Notes and Annotation objects are allowed in ListOfSpecies objects. */
  , SedOnlyParametersInListOfParameters      = 20210 /*!< Only Parameter, Notes and Annotation objects are allowed in ListOfParameters objects. */
  , SedOnlyInitAssignsInListOfInitAssigns    = 20211 /*!< Only InitialAssignment, Notes and Annotation objects are allowed in ListOfInitialAssignments objects. */
  , SedOnlyRulesInListOfRules                = 20212 /*!< Only Rule, Notes and Annotation objects are allowed in ListOfRules objects. */
  , SedOnlyConstraintsInListOfConstraints    = 20213 /*!< Only Constraint, Notes and Annotation objects are allowed in ListOfConstraints objects. */
  , SedOnlyReactionsInListOfReactions        = 20214 /*!< Only Reaction, Notes and Annotation objects are allowed in ListOfReactions objects. */
  , SedOnlyEventsInListOfEvents              = 20215 /*!< Only Event, Notes and Annotation objects are allowed in ListOfEvents objects. */
  , SedL3ConversionFactorOnModel             = 20216 /*!< A 'conversionFactor' attribute value must reference a Parameter object. */
  , SedL3TimeUnitsOnModel                    = 20217 /*!< Invalid 'timeUnits' attribute value. */
  , SedL3VolumeUnitsOnModel                  = 20218 /*!< Invalid 'volumeUnits' attribute value. */
  , SedL3AreaUnitsOnModel                    = 20219 /*!< Invalid 'areaUnits' attribute value. */
  , SedL3LengthUnitsOnModel                  = 20220 /*!< Invalid 'lengthUnits' attribute value. */
  , SedL3ExtentUnitsOnModel                  = 20221 /*!< Invalid 'extentUnits' attribute value. */
  , SedAllowedAttributesOnModel              = 20222 /*!< Invalid attribute found on the Model object. */
  , SedAllowedAttributesOnListOfFuncs        = 20223 /*!< Invalid attribute found on the ListOfFunctionDefinitions object. */
  , SedAllowedAttributesOnListOfUnitDefs     = 20224 /*!< Invalid attribute found on the ListOfUnitDefinitions object. */
  , SedAllowedAttributesOnListOfComps        = 20225 /*!< Invalid attribute found on the ListOfCompartments object. */
  , SedAllowedAttributesOnListOfSpecies      = 20226 /*!< Invalid attribute found on the ListOfSpecies object. */
  , SedAllowedAttributesOnListOfParams       = 20227 /*!< Invalid attribute found on the ListOfParameters object. */
  , SedAllowedAttributesOnListOfInitAssign   = 20228 /*!< Invalid attribute found on the ListOfInitialAssignments object. */
  , SedAllowedAttributesOnListOfRules        = 20229 /*!< Invalid attribute found on the ListOfRules object. */
  , SedAllowedAttributesOnListOfConstraints  = 20230 /*!< Invalid attribute found on the ListOfConstraints object. */
  , SedAllowedAttributesOnListOfReactions    = 20231 /*!< Invalid attribute found on the ListOfReactions object. */
  , SedAllowedAttributesOnListOfEvents       = 20232 /*!< Invalid attribute found on the ListOfEvents object. */
  , SedFunctionDefMathNotLambda              = 20301 /*!< Invalid expression found in the function definition. */
  , SedInvalidApplyCiInLambda                = 20302 /*!< Invalid forward reference in the MathML <code>&lt;apply&gt;</code><code>&lt;ci&gt;</code>...<code>&lt;/ci&gt;</code><code>&lt;/apply&gt;</code> expression. */
  , SedRecursiveFunctionDefinition           = 20303 /*!< Recursive function definitions are not permitted. */
  , SedInvalidCiInLambda                     = 20304 /*!< Invalid <code>&lt;ci&gt;</code> reference found inside the <code>&lt;lambda&gt;</code> mathematical formula. */
  , SedInvalidFunctionDefReturnType          = 20305 /*!< A function's return type must be either a number or a Boolean. */
  , SedOneMathElementPerFunc                 = 20306 /*!< A FunctionDefinition object must contain one <code>&lt;math&gt;</code> element. */
  , SedAllowedAttributesOnFunc               = 20307 /*!< Invalid attribute found on the FunctionDefinition object. */
  , SedInvalidUnitDefId                      = 20401 /*!< Invalid 'id' attribute value for a UnitDefinition object. */
  , SedInvalidSubstanceRedefinition          = 20402 /*!< Invalid redefinition of built-in type 'substance'. */
  , SedInvalidLengthRedefinition             = 20403 /*!< Invalid redefinition of built-in type 'length'. */
  , SedInvalidAreaRedefinition               = 20404 /*!< Invalid redefinition of built-in type name 'area'. */
  , SedInvalidTimeRedefinition               = 20405 /*!< Invalid redefinition of built-in type name 'time'. */
  , SedInvalidVolumeRedefinition             = 20406 /*!< Invalid redefinition of built-in type name 'volume'. */
  , SedVolumeLitreDefExponentNotOne          = 20407 /*!< Must use 'exponent'=1 when defining 'volume' in terms of litres. */
  , SedVolumeMetreDefExponentNot3            = 20408 /*!< Must use 'exponent'=3 when defining 'volume' in terms of metres. */
  , SedEmptyListOfUnits                      = 20409 /*!< An empty list of Unit objects is not permitted in a UnitDefinition object. */
  , SedInvalidUnitKind                       = 20410 /*!< Invalid value for the 'kind' attribute of a UnitDefinition object. */
  , SedOffsetNoLongerValid                   = 20411 /*!< Unit attribute 'offset' is not supported in this Level+Version of Sed. */
  , SedCelsiusNoLongerValid                  = 20412 /*!< Unit name 'Celsius' is not defined in this Level+Version of Sed. */
  , SedEmptyUnitListElement                  = 20413 /*!< A ListOfUnits object must not be empty. */
  , SedOneListOfUnitsPerUnitDef              = 20414 /*!< At most one ListOfUnits object is allowed inside a UnitDefinition object. */
  , SedOnlyUnitsInListOfUnits                = 20415 /*!< Only Unit, Notes and Annotation objects are allowed in ListOfUnits objects. */
  , SedAllowedAttributesOnUnitDefinition     = 20419 /*!< Invalid attribute found on the UnitDefinition object. */
  , SedAllowedAttributesOnListOfUnits        = 20420 /*!< Invalid attribute found on the ListOfUnits object. */
  , SedAllowedAttributesOnUnit               = 20421 /*!< Invalid attribute found on the Unit object. */
  , SedZeroDimensionalCompartmentSize        = 20501 /*!< Invalid use of the 'size' attribute for a zero-dimensional compartment. */
  , SedZeroDimensionalCompartmentUnits       = 20502 /*!< Invalid use of the 'units' attribute for a zero-dimensional compartment. */
  , SedZeroDimensionalCompartmentConst       = 20503 /*!< Zero-dimensional compartments must be defined to be constant. */
  , SedUndefinedOutsideCompartment           = 20504 /*!< Invalid value for the 'outside' attribute of a Compartment object. */
  , SedRecursiveCompartmentContainment       = 20505 /*!< Recursive nesting of compartments via the 'outside' attribute is not permitted. */
  , SedZeroDCompartmentContainment           = 20506 /*!< Invalid nesting of zero-dimensional compartments. */
  , SedInvalid1DCompartmentUnits             = 20507 /*!< Invalid value for the 'units' attribute of a one-dimensional compartment. */
  , SedInvalid2DCompartmentUnits             = 20508 /*!< Invalid value for the 'units' attribute of a two-dimensional compartment. */
  , SedInvalid3DCompartmentUnits             = 20509 /*!< Invalid value for the 'units' attribute of a three-dimensional compartment. */
  , SedInvalidCompartmentTypeRef             = 20510 /*!< Invalid value for the 'compartmentType' attribute of a compartment. */
  , SedOneDimensionalCompartmentUnits        = 20511 /*!< No units defined for 1-D compartment. */
  , SedTwoDimensionalCompartmentUnits        = 20512 /*!< No units defined for 2-D compartment. */
  , SedThreeDimensionalCompartmentUnits      = 20513 /*!< No units defined for 3-D Compartment object. */
  , SedAllowedAttributesOnCompartment        = 20517 /*!< Invalid attribute found on Compartment object. */
  , SedNoUnitsOnCompartment                  = 20518 /*!< No units defined for Compartment object. */
  , SedInvalidSpeciesCompartmentRef          = 20601 /*!< Invalid value found for Species 'compartment' attribute. */
  , SedHasOnlySubsNoSpatialUnits             = 20602 /*!< Attribute 'spatialSizeUnits' must not be set if 'hasOnlySubstanceUnits'='true'. */
  , SedNoSpatialUnitsInZeroD                 = 20603 /*!< Attribute 'spatialSizeUnits' must not be set if the compartment is zero-dimensional. */
  , SedNoConcentrationInZeroD                = 20604 /*!< Attribute 'initialConcentration' must not be set if the compartment is zero-dimensional. */
  , SedSpatialUnitsInOneD                    = 20605 /*!< Invalid value for 'spatialSizeUnits' attribute of a one-dimensional compartment. */
  , SedSpatialUnitsInTwoD                    = 20606 /*!< Invalid value for the 'spatialSizeUnits' attribute of a two-dimensional compartment. */
  , SedSpatialUnitsInThreeD                  = 20607 /*!< Invalid value for the 'spatialSizeUnits' attribute of a three-dimensional compartment. */
  , SedInvalidSpeciesSusbstanceUnits         = 20608 /*!< Invalid value for a Species 'units' attribute. */
  , SedBothAmountAndConcentrationSet         = 20609 /*!< Cannot set both 'initialConcentration' and 'initialAmount' attributes simultaneously. */
  , SedNonBoundarySpeciesAssignedAndUsed     = 20610 /*!< Cannot use a non-boundary species in both reactions and rules simultaneously. */
  , SedNonConstantSpeciesUsed                = 20611 /*!< Cannot use a constant, non-boundary species as a reactant or product. */
  , SedInvalidSpeciesTypeRef                 = 20612 /*!< Invalid value for the 'speciesType' attribute of a species. */
  , SedMultSpeciesSameTypeInCompartment      = 20613 /*!< Cannot have multiple species of the same species type in the same compartment. */
  , SedMissingSpeciesCompartment             = 20614 /*!< Missing value for the 'compartment' attribute. */
  , SedSpatialSizeUnitsRemoved               = 20615 /*!< Attribute 'spatialSizeUnits' is not supported in this Level+Version of Sed. */
  , SedSubstanceUnitsOnSpecies               = 20616 /*!< No substance units defined for the species. */
  , SedConversionFactorOnSpecies             = 20617 /*!< Invalid value for the 'conversionFactor' attribute. */
  , SedAllowedAttributesOnSpecies            = 20623 /*!< Invalid attribute found on Species object. */
  , SedInvalidParameterUnits                 = 20701 /*!< Invalid value for the 'units' attribute of a Parameter object. */
  , SedParameterUnits                        = 20702 /*!< No units defined for the parameter. */
  , SedConversionFactorMustConstant          = 20705 /*!< A conversion factor must reference a Parameter object declared to be a constant. */
  , SedAllowedAttributesOnParameter          = 20706 /*!< Invalid attribute found on Parameter object. */
  , SedInvalidInitAssignSymbol               = 20801 /*!< Invalid value for the 'symbol' attribute of an InitialAssignment object. */
  , SedMultipleInitAssignments               = 20802 /*!< Multiple initial assignments for the same 'symbol' value are not allowed. */
  , SedInitAssignmentAndRuleForSameId        = 20803 /*!< Cannot set a value using both an initial assignment and an assignment rule simultaneously. */
  , SedOneMathElementPerInitialAssign        = 20804 /*!< An InitialAssignment object must contain one <code>&lt;math&gt;</code> element. */
  , SedAllowedAttributesOnInitialAssign      = 20805 /*!< Invalid attribute found on an InitialAssignment object. */
  , SedInvalidAssignRuleVariable             = 20901 /*!< Invalid value for the 'variable' attribute of an AssignmentRule object. */
  , SedInvalidRateRuleVariable               = 20902 /*!< Invalid value for the 'variable' attribute of a RateRule object. */
  , SedAssignmentToConstantEntity            = 20903 /*!< An assignment rule cannot assign an entity declared to be constant. */
  , SedRateRuleForConstantEntity             = 20904 /*!< A rate rule cannot assign an entity declared to be constant. */
  , SedRepeatedRule10304                     = 20905
  , SedCircularRuleDependency                = 20906 /*!< Circular dependencies involving rules and reactions are not permitted. */
  , SedOneMathElementPerRule                 = 20907 /*!< A rule object must contain one <code>&lt;math&gt;</code> element. */
  , SedAllowedAttributesOnAssignRule         = 20908 /*!< Invalid attribute found on an AssignmentRule object. */
  , SedAllowedAttributesOnRateRule           = 20909 /*!< Invalid attribute found on a RateRule object. */
  , SedAllowedAttributesOnAlgRule            = 20910 /*!< Invalid attribute found on an AlgebraicRule object. */
  , SedConstraintMathNotBoolean              = 21001 /*!< A Constraint object's <code>&lt;math&gt;</code> must evaluate to a Boolean value. */
  , SedIncorrectOrderInConstraint            = 21002 /*!< Subobjects inside the Constraint object are not in the prescribed order. */
  , SedConstraintNotInXHTMLNamespace         = 21003 /*!< A Constraint's Message subobject must be in the XHTML XML namespace. */
  , SedConstraintContainsXMLDecl             = 21004 /*!< XML declarations are not permitted within Constraint's Message objects. */
  , SedConstraintContainsDOCTYPE             = 21005 /*!< XML DOCTYPE elements are not permitted within Constraint's Message objects. */
  , SedInvalidConstraintContent              = 21006 /*!< Invalid content for a Constraint object's Message object. */
  , SedOneMathElementPerConstraint           = 21007 /*!< A Constraint object must contain one <code>&lt;math&gt;</code> element. */
  , SedOneMessageElementPerConstraint        = 21008 /*!< A Constraint object must contain one Message subobject. */
  , SedAllowedAttributesOnConstraint         = 21009 /*!< Invalid attribute found on Constraint object. */
  , SedNoReactantsOrProducts                 = 21101 /*!< Cannot have a reaction with neither reactants nor products. */
  , SedIncorrectOrderInReaction              = 21102 /*!< Subobjects inside the Reaction object are not in the prescribed order. */
  , SedEmptyListInReaction                   = 21103 /*!< Reaction components, if present, cannot be empty. */
  , SedInvalidReactantsProductsList          = 21104 /*!< Invalid object found in the list of reactants or products. */
  , SedInvalidModifiersList                  = 21105 /*!< Invalid object found in the list of modifiers. */
  , SedOneSubElementPerReaction              = 21106 /*!< A Reaction object can only contain one of each allowed type of object. */
  , SedCompartmentOnReaction                 = 21107 /*!< Invalid value for the Reaction 'compartment' attribute. */
  , SedAllowedAttributesOnReaction           = 21110 /*!< Invalid attribute for a Reaction object. */
  , SedInvalidSpeciesReference               = 21111 /*!< Invalid 'species' attribute value in SpeciesReference object. */
  , SedRepeatedRule20611                     = 21112
  , SedBothStoichiometryAndMath              = 21113 /*!< The 'stoichiometry' attribute and StoichiometryMath subobject are mutually exclusive. */
  , SedAllowedAttributesOnSpeciesReference   = 21116 /*!< Invalid attribute found on the SpeciesReference object. */
  , SedAllowedAttributesOnModifier           = 21117 /*!< Invalid attribute found on the ModifierSpeciesReference object. */
  , SedUndeclaredSpeciesRef                  = 21121 /*!< Unknown species referenced in the kinetic law <code>&lt;math&gt;</code> formula. */
  , SedIncorrectOrderInKineticLaw            = 21122 /*!< Incorrect ordering of components in the KineticLaw object. */
  , SedEmptyListInKineticLaw                 = 21123 /*!< The list of parameters, if present, cannot be empty. */
  , SedNonConstantLocalParameter             = 21124 /*!< Parameters local to a KineticLaw object must have a 'constant' attribute value of 'true'. */
  , SedSubsUnitsNoLongerValid                = 21125 /*!< Attribute 'substanceUnits' is not supported in this Level+Version of Sed. */
  , SedTimeUnitsNoLongerValid                = 21126 /*!< Attribute 'timeUnits' is not supported in this Level+Version of Sed. */
  , SedOneListOfPerKineticLaw                = 21127 /*!< Only one ListOfLocalParameters object is permitted within a KineticLaw object. */
  , SedOnlyLocalParamsInListOfLocalParams    = 21128 /*!< Only LocalParameter, Notes and Annotation objects are allowed in ListOfLocalParameter objects. */
  , SedAllowedAttributesOnListOfLocalParam   = 21129 /*!< Invalid attribute found on the ListOfLocalParameters object. */
  , SedOneMathPerKineticLaw                  = 21130 /*!< Only one <code>&lt;math&gt;</code> element is allowed in a KineticLaw object. */
  , SedUndeclaredSpeciesInStoichMath         = 21131 /*!< Unknown species referenced in the StoichiometryMath object's <code>&lt;math&gt;</code> formula. */
  , SedAllowedAttributesOnKineticLaw         = 21132 /*!< Invalid attribute found on the KineticLaw object. */
  , SedAllowedAttributesOnListOfSpeciesRef   = 21150 /*!< Invalid attribute found on the ListOfSpeciesReferences object. */
  , SedAllowedAttributesOnListOfMods         = 21151 /*!< Invalid attribute found on the ListOfModifiers object. */
  , SedAllowedAttributesOnLocalParameter     = 21172 /*!< Invalid attribute found on the LocalParameter object. */
  , SedMissingTriggerInEvent                 = 21201 /*!< The Event object is missing a Trigger subobject. */
  , SedTriggerMathNotBoolean                 = 21202 /*!< A Trigger object's <code>&lt;math&gt;</code> expression must evaluate to a Boolean value. */
  , SedMissingEventAssignment                = 21203 /*!< The Event object is missing an EventAssignment subobject. */
  , SedTimeUnitsEvent                        = 21204 /*!< Units referenced by 'timeUnits' attribute are not compatible with units of time. */
  , SedIncorrectOrderInEvent                 = 21205 /*!< Incorrect ordering of components in Event object. */
  , SedValuesFromTriggerTimeNeedDelay        = 21206 /*!< Attribute 'useValuesFromTriggerTime'='false', but the Event object does not define a delay. */
  , SedDelayNeedsValuesFromTriggerTime       = 21207 /*!< The use of a Delay object requires the Event attribute 'useValuesFromTriggerTime'. */
  , SedOneMathPerTrigger                     = 21209 /*!< A Trigger object must have one <code>&lt;math&gt;</code> element. */
  , SedOneMathPerDelay                       = 21210 /*!< A Delay object must have one <code>&lt;math&gt;</code> element. */
  , SedInvalidEventAssignmentVariable        = 21211 /*!< Invalid 'variable' attribute value in Event object. */
  , SedEventAssignmentForConstantEntity      = 21212 /*!< An EventAssignment object cannot assign to a component having attribute 'constant'='true'. */
  , SedOneMathPerEventAssignment             = 21213 /*!< An EventAssignment object must have one <code>&lt;math&gt;</code> element. */
  , SedAllowedAttributesOnEventAssignment    = 21214 /*!< Invalid attribute found on the EventAssignment object. */
  , SedOnlyOneDelayPerEvent                  = 21221 /*!< An Event object can only have one Delay subobject. */
  , SedOneListOfEventAssignmentsPerEvent     = 21222 /*!< An Event object can only have one ListOfEventAssignments subobject. */
  , SedOnlyEventAssignInListOfEventAssign    = 21223 /*!< Only EventAssignment, Notes and Annotation objects are allowed in ListOfEventAssignments. */
  , SedAllowedAttributesOnListOfEventAssign  = 21224 /*!< Invalid attribute found on the ListOfEventAssignments object. */
  , SedAllowedAttributesOnEvent              = 21225 /*!< Invalid attribute found on the Event object. */
  , SedAllowedAttributesOnTrigger            = 21226 /*!< Invalid attribute found on the Trigger object. */
  , SedAllowedAttributesOnDelay              = 21227 /*!< Invalid attribute found on the Delay object. */
  , SedPersistentNotBoolean                  = 21228 /*!< The Trigger attribute 'persistent' must evaluate to a Boolean value. */
  , SedInitialValueNotBoolean                = 21229 /*!< The Trigger attribute 'initialValue' must evaluate to a Boolean value. */
  , SedOnlyOnePriorityPerEvent               = 21230 /*!< An Event object can only have one Priority subobject. */
  , SedOneMathPerPriority                    = 21231 /*!< A Priority object must have one <code>&lt;math&gt;</code> element. */
  , SedAllowedAttributesOnPriority           = 21232 /*!< Invalid attribute found on the Priority object. */
  , SedGeneralWarningNotSpecified            = 29999 /*!< Unknown error. */
  , SedCompartmentShouldHaveSize             = 80501 /*!< It's best to define a size for every compartment in a model. */
  , SedSpeciesShouldHaveValue                = 80601 /*!< It's best to define an initial amount or initial concentration for every species in a model. */
  , SedParameterShouldHaveUnits              = 80701 /*!< It's best to declare units for every parameter in a model. */
  , SedLocalParameterShadowsId               = 81121 /*!< Local parameters defined within a kinetic law shadow global object symbols. */
  , SedLibSedAdditionalCodesLowerBound      = 90000 /*!< Lower boundary of libSed-specific diagnostic codes. */
  , SedCannotConvertToL1V1                   = 90001 /*!< Cannot convert to Sed Level 1 Version 1. */
  , SedNoEventsInL1                          = 91001 /*!< Sed Level 1 does not support events. */
  , SedNoFunctionDefinitionsInL1             = 91002 /*!< Sed Level 1 does not support function definitions. */
  , SedNoConstraintsInL1                     = 91003 /*!< Sed Level 1 does not support constraints. */
  , SedNoInitialAssignmentsInL1              = 91004 /*!< Sed Level 1 does not support initial assignments. */
  , SedNoSpeciesTypesInL1                    = 91005 /*!< Sed Level 1 does not support species types. */
  , SedNoCompartmentTypeInL1                 = 91006 /*!< Sed Level 1 does not support compartment types. */
  , SedNoNon3DCompartmentsInL1               = 91007 /*!< Sed Level 1 only supports three-dimensional compartments. */
  , SedNoFancyStoichiometryMathInL1          = 91008 /*!< Sed Level 1 does not support non-integer nor non-rational stoichiometry formulas. */
  , SedNoNonIntegerStoichiometryInL1         = 91009 /*!< Sed Level 1 does not support non-integer 'stoichiometry' attribute values. */
  , SedNoUnitMultipliersOrOffsetsInL1        = 91010 /*!< Sed Level 1 does not support multipliers or offsets in unit definitions. */
  , SedSpeciesCompartmentRequiredInL1        = 91011 /*!< In Sed Level 1, a value for 'compartment' is mandatory in species definitions. */
  , SedNoSpeciesSpatialSizeUnitsInL1         = 91012 /*!< Sed Level 1 does not support species 'spatialSizeUnits' settings. */
  , SedNoSBOTermsInL1                        = 91013 /*!< Sed Level 1 does not support the 'sboTerm' attribute. */
  , SedStrictUnitsRequiredInL1               = 91014 /*!< Sed Level 1 requires strict unit consistency. */
  , SedConversionFactorNotInL1               = 91015 /*!< Sed Level 1 does not support the 'conversionFactor' attribute. */
  , SedCompartmentNotOnL1Reaction            = 91016 /*!< Sed Level 1 does not support the 'compartment' attribute on Reaction objects. */
  , SedExtentUnitsNotSubstance               = 91017 /*!< Units of extent must be compatible with units of substance. */
  , SedGlobalUnitsNotDeclared                = 91018 /*!< Global units must be refer to unit kind or unitDefinition.. */
  , SedHasOnlySubstanceUnitsNotinL1          = 91019 /*!< The concept of hasOnlySubstanceUnits was not available in Sed Level 1.. */
  , SedAvogadroNotSupported                  = 91020 /*!< Avogadro not supported in Levels 2 and 1.. */
  , SedNoConstraintsInL2v1                   = 92001 /*!< Sed Level 2 Version 1 does not support Constraint objects. */
  , SedNoInitialAssignmentsInL2v1            = 92002 /*!< Sed Level 2 Version 1 does not support InitialAssignment objects. */
  , SedNoSpeciesTypeInL2v1                   = 92003 /*!< Sed Level 2 Version 1 does not support SpeciesType objects. */
  , SedNoCompartmentTypeInL2v1               = 92004 /*!< Sed Level 2 Version 1 does not support CompartmentType objects. */
  , SedNoSBOTermsInL2v1                      = 92005 /*!< Sed Level 2 Version 1 does not support the 'sboTerm' attribute. */
  , SedNoIdOnSpeciesReferenceInL2v1          = 92006 /*!< Sed Level 2 Version 1 does not support the 'id' attribute on SpeciesReference objects. */
  , SedNoDelayedEventAssignmentInL2v1        = 92007 /*!< Sed Level 2 Version 1 does not support the 'useValuesFromTriggerTime' attribute. */
  , SedStrictUnitsRequiredInL2v1             = 92008 /*!< Sed Level 2 Version 1 requires strict unit consistency. */
  , SedIntegerSpatialDimensions              = 92009 /*!< Sed Level 2 Version 1 requires that compartments have spatial dimensions of 0-3. */
  , SedStoichiometryMathNotYetSupported      = 92010 /*!< Conversion to StoichiometryMath objects not yet supported. */
  , SedPriorityLostFromL3                    = 92011 /*!< Sed Level 2 Version 1 does not support priorities on Event objects. */
  , SedNonPersistentNotSupported             = 92012 /*!< Sed Level 2 Version 1 does not support the 'persistent' attribute on Trigger objects. */
  , SedInitialValueFalseEventNotSupported    = 92013 /*!< Sed Level 2 Version 1 does not support the 'initialValue' attribute on Trigger objects. */
  , SedSBOTermNotUniversalInL2v2             = 93001 /*!< The 'sboTerm' attribute is invalid for this component in Sed Level 2 Version 2. */
  , SedNoUnitOffsetInL2v2                    = 93002 /*!< This Level+Version of Sed does not support the 'offset' attribute on Unit objects. */
  , SedNoKineticLawTimeUnitsInL2v2           = 93003 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on KineticLaw objects. */
  , SedNoKineticLawSubstanceUnitsInL2v2      = 93004 /*!< This Level+Version of Sed does not support the 'substanceUnits' attribute on KineticLaw objects. */
  , SedNoDelayedEventAssignmentInL2v2        = 93005 /*!< This Level+Version of Sed does not support the 'useValuesFromTriggerTime' attribute. */
  , SedModelSBOBranchChangedBeyondL2v2       = 93006 /*!< The allowable 'sboTerm' attribute values for Model objects differ for this Sed Level+Version. */
  , SedStrictUnitsRequiredInL2v2             = 93007 /*!< Sed Level 2 Version 2 requires strict unit consistency. */
  , SedStrictSBORequiredInL2v2               = 93008 /*!< Sed Level 2 Version 2 requires strict SBO term consistency. */
  , SedDuplicateAnnotationInvalidInL2v2      = 93009 /*!< Duplicate top-level annotations are invalid in Sed Level 2 Version 2. */
  , SedNoUnitOffsetInL2v3                    = 94001 /*!< This Level+Version of Sed does not support the 'offset' attribute on Unit objects. */
  , SedNoKineticLawTimeUnitsInL2v3           = 94002 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on KineticLaw objects. */
  , SedNoKineticLawSubstanceUnitsInL2v3      = 94003 /*!< This Level+Version of Sed does not support the 'substanceUnits' attribute on KineticLaw objects. */
  , SedNoSpeciesSpatialSizeUnitsInL2v3       = 94004 /*!< This Level+Version of Sed does not support the 'spatialSizeUnit' attribute on Species objects. */
  , SedNoEventTimeUnitsInL2v3                = 94005 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on Event objects. */
  , SedNoDelayedEventAssignmentInL2v3        = 94006 /*!< This Level+Version of Sed does not support the 'useValuesFromTriggerTime' attribute. */
  , SedModelSBOBranchChangedBeyondL2v3       = 94007 /*!< The allowable 'sboTerm' attribute values for Model objects differ for this Sed Level+Version. */
  , SedStrictUnitsRequiredInL2v3             = 94008 /*!< Sed Level 2 Version 3 requires strict unit consistency. */
  , SedStrictSBORequiredInL2v3               = 94009 /*!< Sed Level 2 Version 3 requires strict SBO term consistency. */
  , SedDuplicateAnnotationInvalidInL2v3      = 94010 /*!< Duplicate top-level annotations are invalid in Sed Level 2 Version 3. */
  , SedNoUnitOffsetInL2v4                    = 95001 /*!< This Level+Version of Sed does not support the 'offset' attribute on Unit objects. */
  , SedNoKineticLawTimeUnitsInL2v4           = 95002 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on KineticLaw objects. */
  , SedNoKineticLawSubstanceUnitsInL2v4      = 95003 /*!< This Level+Version of Sed does not support the 'substanceUnits' attribute on KineticLaw objects. */
  , SedNoSpeciesSpatialSizeUnitsInL2v4       = 95004 /*!< This Level+Version of Sed does not support the 'spatialSizeUnit' attribute on Species objects. */
  , SedNoEventTimeUnitsInL2v4                = 95005 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on Event objects. */
  , SedModelSBOBranchChangedInL2v4           = 95006 /*!< The allowable 'sboTerm' attribute values for Model objects differ for this Sed Level+Version. */
  , SedDuplicateAnnotationInvalidInL2v4      = 95007 /*!< Duplicate top-level annotations are invalid in Sed Level 2 Version 4. */
  , SedNoSpeciesTypeInL3v1                   = 96001 /*!< Sed Level 3 Version 1 does not support SpeciesType objects. */
  , SedNoCompartmentTypeInL3v1               = 96002 /*!< Sed Level 3 Version 1 does not support CompartmentType objects. */
  , SedNoUnitOffsetInL3v1                    = 96003 /*!< This Level+Version of Sed does not support the 'offset' attribute on Unit objects. */
  , SedNoKineticLawTimeUnitsInL3v1           = 96004 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on KineticLaw objects. */
  , SedNoKineticLawSubstanceUnitsInL3v1      = 96005 /*!< This Level+Version of Sed does not support the 'substanceUnits' attribute on KineticLaw objects. */
  , SedNoSpeciesSpatialSizeUnitsInL3v1       = 96006 /*!< This Level+Version of Sed does not support the 'spatialSizeUnit' attribute on Species objects. */
  , SedNoEventTimeUnitsInL3v1                = 96007 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on Event objects. */
  , SedModelSBOBranchChangedInL3v1           = 96008 /*!< The allowable 'sboTerm' attribute values for Model objects differ for this Sed Level+Version. */
  , SedDuplicateAnnotationInvalidInL3v1      = 96009 /*!< Duplicate top-level annotations are invalid in Sed Level 3 Version 1. */
  , SedNoCompartmentOutsideInL3v1            = 96010 /*!< This Level+Version of Sed does not support the 'outside' attribute on Compartment objects. */
  , SedNoStoichiometryMathInL3v1             = 96011 /*!< This Level+Version of Sed does not support the StoichiometryMath object. */
  , SedInvalidSedLevelVersion               = 99101 /*!< Unknown Level+Version combination of Sed. */
  , SedAnnotationNotesNotAllowedLevel1       = 99104 /*!< Annotation objects on the Sed container element are not permitted in Sed Level 1. */
  , SedInvalidRuleOrdering                   = 99106 /*!< Invalid ordering of rules. */
  , SedRequiredPackagePresent                = 99107 /*!< The Sed document requires an Sed Level 3 package unavailable in this software. */
  , SedUnrequiredPackagePresent              = 99108 /*!< The Sed document uses an Sed Level 3 package unavailable in this software. */
  , SedPackageRequiredShouldBeFalse          = 99109 /*!< This package expects required to be false.. */
  , SedSubsUnitsAllowedInKL                  = 99127 /*!< Disallowed value for attribute 'substanceUnits' on KineticLaw object. */
  , SedTimeUnitsAllowedInKL                  = 99128 /*!< Disallowed value for attribute 'timeUnits' on KineticLaw object. */
  , SedFormulaInLevel1KL                     = 99129 /*!< Only predefined functions are allowed in Sed Level 1 formulas. */
  , SedL3SubstanceUnitsOnModel               = 99130 /*!< Invalid 'substanceUnits' attribute value. */
  , SedTimeUnitsRemoved                      = 99206 /*!< This Level+Version of Sed does not support the 'timeUnits' attribute on Event objects. */
  , SedBadMathML                             = 99219 /*!< Invalid MathML expression. */
  , SedFailedMathMLReadOfDouble              = 99220 /*!< Missing or invalid floating-point number in MathML expression. */
  , SedFailedMathMLReadOfInteger             = 99221 /*!< Missing or invalid integer in MathML expression. */
  , SedFailedMathMLReadOfExponential         = 99222 /*!< Missing or invalid exponential expression in MathML. */
  , SedFailedMathMLReadOfRational            = 99223 /*!< Missing or invalid rational expression in MathML. */
  , SedBadMathMLNodeType                     = 99224 /*!< Invalid MathML element. */
  , SedNoTimeSymbolInFunctionDef             = 99301 /*!< Use of <code>&lt;csymbol&gt;</code> for 'time' not allowed within FunctionDefinition objects. */
  , SedNoBodyInFunctionDef                   = 99302 /*!< There must be a <code>&lt;lambda&gt;</code> body within the <code>&lt;math&gt;</code> element of a FunctionDefinition object. */
  , SedDanglingUnitSIdRef                    = 99303 /*!< Units must refer to valid unit or unitDefinition. */
  , SedRDFMissingAboutTag                    = 99401 /*!< RDF missing the <code>&lt;about&gt;</code> tag.. */
  , SedRDFEmptyAboutTag                      = 99402 /*!< RDF empty <code>&lt;about&gt;</code> tag.. */
  , SedRDFAboutTagNotMetaid                  = 99403 /*!< RDF <code>&lt;about&gt;</code> tag is not metaid.. */
  , SedRDFNotCompleteModelHistory            = 99404 /*!< RDF does not contain valid ModelHistory.. */
  , SedRDFNotModelHistory                    = 99405 /*!< RDF does not result in a ModelHistory.. */
  , SedAnnotationNotElement                  = 99406 /*!< Annotation must contain element.. */
  , SedInconsistentArgUnitsWarnings          = 99502
  , SedInconsistentPowerUnitsWarnings        = 99503
  , SedInconsistentExponUnitsWarnings        = 99504
  , SedUndeclaredUnits                       = 99505 /*!< Missing unit declarations on parameters or literal numbers in expression. */
  , SedUndeclaredTimeUnitsL3                 = 99506 /*!< Unable to verify consistency of units: the unit of time has not been declared. */
  , SedUndeclaredExtentUnitsL3               = 99507 /*!< Unable to verify consistency of units: the units of reaction extent have not been declared. */
  , SedUndeclaredObjectUnitsL3               = 99508 /*!< Unable to verify consistency of units: encountered a model entity with no declared units. */
  , SedUnrecognisedSBOTerm                   = 99701 /*!< Unrecognized 'sboTerm' attribute value. */
  , SedObseleteSBOTerm                       = 99702 /*!< Obsolete 'sboTerm' attribute value. */
  , SedIncorrectCompartmentSpatialDimensions = 99901 /*!< In Sed Level 1, only three-dimensional compartments are allowed. */
  , SedCompartmentTypeNotValidAttribute      = 99902 /*!< CompartmentType objects are not available in this Level+Version of Sed. */
  , SedConstantNotValidAttribute             = 99903 /*!< This Level+Version of Sed does not support the 'constant' attribute on this component. */
  , SedMetaIdNotValidAttribute               = 99904 /*!< Attribute 'metaid' is not available in Sed Level 1. */
  , SedSBOTermNotValidAttributeBeforeL2V3    = 99905 /*!< The 'sboTerm' attribute is not available on this component before Sed Level 2 Version 3. */
  , SedInvalidL1CompartmentUnits             = 99906 /*!< Invalid units for a compartment in Sed Level 1. */
  , SedL1V1CompartmentVolumeReqd             = 99907 /*!< In Sed Level 1, a compartment's volume must be specified. */
  , SedCompartmentTypeNotValidComponent      = 99908 /*!< CompartmentType objects are not available in this Level+Version of Sed. */
  , SedConstraintNotValidComponent           = 99909 /*!< Constraint objects are not available in this Level+Version of Sed. */
  , SedEventNotValidComponent                = 99910 /*!< Event objects are not available in this Level+Version of Sed. */
  , SedSBOTermNotValidAttributeBeforeL2V2    = 99911 /*!< The 'sboTerm' attribute is invalid for this component before Level 2 Version 2. */
  , SedFuncDefNotValidComponent              = 99912 /*!< FunctionDefinition objects are not available in this Level+Version of Sed. */
  , SedInitialAssignNotValidComponent        = 99913 /*!< InitialAssignment objects are not available in this Level+Version of Sed. */
  , SedVariableNotValidAttribute             = 99914 /*!< Attribute 'variable' is not available on this component in this Level+Version of Sed. */
  , SedUnitsNotValidAttribute                = 99915 /*!< Attribute 'units' is not available on this component in this Level+Version of Sed. */
  , SedConstantSpeciesNotValidAttribute      = 99916 /*!< Attribute 'constant' is not available on Species objects in Sed Level 1. */
  , SedSpatialSizeUnitsNotValidAttribute     = 99917 /*!< Attribute 'spatialSizeUnits' is not available on Species objects in Sed Level 1. */
  , SedSpeciesTypeNotValidAttribute          = 99918 /*!< Attribute 'speciesType' is not available on Species objects in Sed Level 1. */
  , SedHasOnlySubsUnitsNotValidAttribute     = 99919 /*!< Attribute 'hasOnlySubstanceUnits' is not available on Species objects in Sed Level 1. */
  , SedIdNotValidAttribute                   = 99920 /*!< Attribute 'id' is not available on SpeciesReference objects in Sed Level 1. */
  , SedNameNotValidAttribute                 = 99921 /*!< Attribute 'name' is not available on SpeciesReference objects in Sed Level 1. */
  , SedSpeciesTypeNotValidComponent          = 99922 /*!< The SpeciesType object is not supported in Sed Level 1. */
  , SedStoichiometryMathNotValidComponent    = 99923 /*!< The StoichiometryMath object is not supported in Sed Level 1. */
  , SedMultiplierNotValidAttribute           = 99924 /*!< Attribute 'multiplier' on Unit objects is not supported in Sed Level 1. */
  , SedOffsetNotValidAttribute               = 99925 /*!< Attribute 'offset' on Unit objects is only available in Sed Level 2 Version 1. */
  , SedL3SpatialDimensionsUnset              = 99926 /*!< No value given for 'spatialDimensions' attribute; assuming a value of 3. */
  , SedUnknownCoreAttribute                  = 99994 /*!< Encountered an unknow attribute in core ns */
  , SedUnknownPackageAttribute               = 99995 /*!< Encountered an unknow attribute in package ns  */
  , SedPackageConversionNotSupported         = 99996 /*!< Conversion of Sed Level 3 package constructs is not yet supported. */
  , SedInvalidTargetLevelVersion             = 99997 /*!< The requested Sed Level/Version combination is not known to exist. */
  , SedL3NotSupported                        = 99998 /*!< Sed Level 3 is not yet supported. */
  , SedCodesUpperBound                   = 99999
} SedErrorCode_t;


/**
 * Category codes for SedError diagnostics.
 *
 * Note that these are distinct from XMLError's category codes.  User
 * programs receiving an SedError object can use this distinction to
 * check whether the error represents a low-level XML problem or an
 * Sed problem.
 *
 * @see #XMLErrorCategory_t
 */
typedef enum
{
  LIBSEDML_CAT_INTERNAL,
  LIBSEDML_CAT_SEDML
  /*!< General Sed error  not falling into another category below. */

  , LIBSEDML_CAT_SEDML_L1_COMPAT
  /*!< Category of errors that can only occur during attempted
   * translation from one Level/Version of Sed to another.  This
   * particular category applies to errors encountered while trying to
   * convert a model from Sed Level&nbsp;2 to Sed Level&nbsp;1. */

  , LIBSEDML_CAT_GENERAL_CONSISTENCY
  /*!< Category of errors that can occur while validating general Sed
   * constructs.  With respect to the Sed specification, these concern
   * failures in applying the validation rules numbered 2xxxx in the
   * Level&nbsp;2 Versions&nbsp;2&ndash;4
   * and Level&nbsp;3 Version&nbsp;1 specifications. */

  , LIBSEDML_CAT_IDENTIFIER_CONSISTENCY
  /*!< Category of errors that can occur while validating symbol
   * identifiers in a model.  With respect to the Sed specification,
   * these concern failures in applying the validation rules numbered
   * 103xx in the Level&nbsp;2 Versions&nbsp;2&ndash;4
   * and Level&nbsp;3 Version&nbsp;1 specifications. */


  , LIBSEDML_CAT_MATHML_CONSISTENCY
  /*!< Category of errors that can occur while validating MathML formulas
   * in a model.  With respect to the Sed specification, these concern
   * failures in applying the validation rules numbered 102xx in the
   * Level&nbsp;2 Versions&nbsp;2&ndash;4
   * and Level&nbsp;3 Version&nbsp;1 specifications. */

  , LIBSEDML_CAT_INTERNAL_CONSISTENCY
  /*!< Category of errors that can occur while validating libSed's
   * internal representation of Sed constructs. (These are tests
   * performed by libSed and do not have equivalent Sed validation
   * rules.)  */


} SedErrorCategory_t;


/**
 * Severity codes for SedError diagnostics.
 *
 * These are distinct from XMLError's severity codes.
 *
 * @see XMLErrorSeverity_t
 */
typedef enum
{
  /** @cond doxygen-libsbml-internal **/

  /* The following are used internally in SedErrorTable, but publicly,
   * we only report one of the 4 XMLError_Severity values.  Translation
   * of the codes is done in SedError.cpp.
   */
  LIBSEDML_SEV_ERROR = LIBSBML_SEV_ERROR,
  LIBSEDML_SEV_FATAL = LIBSBML_SEV_FATAL,
  LIBSEDML_SEV_WARNING = LIBSBML_SEV_WARNING,

  LIBSEDML_SEV_SCHEMA_ERROR    = (LIBSBML_SEV_FATAL + 1)
                                 /*!< The XML content does not conform to
                                  * the relevant version of the Sed XML
                                  * Schema.  The content is not valid Sed. */

  , LIBSEDML_SEV_GENERAL_WARNING
  /*!< The XML content is invalid for some
   * levels/versions of Sed, and while it
   * may be valid in others, it is something
   * that is best avoided anyway.  LibSed
   * will issue warnings in those cases it
   * can recognize. */

  , LIBSEDML_SEV_NOT_APPLICABLE
  /*!< This error code is only a placeholder
   * for errors that have relevance to some
   * versions of Sed but not others. */

  /** @endcond **/
} SedErrorSeverity_t;

END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#ifdef __cplusplus

LIBSEDML_CPP_NAMESPACE_BEGIN

class LIBSEDML_EXTERN SedError : public XMLError
{
public:

  /**
   * Creates a new SedError to report that something occurred during Sed
   * processing.
   *
   * When a libSed operation on Sed content results in a warning, error
   * or other diagnostic, the issue is reported as an SedError object.
   * SedError objects have identification numbers to indicate the nature
   * of the exception.  @if clike These numbers are drawn from
   * the enumeration <a class="el"
   * href="#SedErrorCode_t">
   * SedErrorCode_t</a>.  @endif@if java These numbers are
   * defined as unsigned integer constants in the file
   * "libsbmlConstants.html".  See the <a class="el"
   * href="#SedErrorCode_t">top of this documentation page</a> for a table
   * listing the possible values and their meanings. @endif@if python These
   * numbers are defined as unsigned integer constants in the interface
   * class @link libsbml libsbml@endlink.  See the <a class="el"
   * href="#SedErrorCode_t">top of this documentation page</a> for a table
   * listing the possible values and their meanings. @endif@~ The argument
   * @p errorId to this constructor @em can be (but does not have to be) a
   * value from this @if clike enumeration. If it @em is a value
   * from <a class="el" href="#SedErrorCode_t">SedErrorCode_t</a>, the
   * SedError class assumes the error is a low-level system or Sed layer
   * error and <em>prepends</em> a built-in, predefined error message to
   * any string passed in the argument @p details to this constructor.  In
   * addition, all <a class="el"
   * href="#SedErrorCode_t">SedErrorCode_t</a> errors have associated
   * values for the @p severity and @p category codes, and these fields are
   * filled-in as well from the enumerations <a class="el"
   * href="#SedErrorSeverity_t">SedErrorSeverity_t</a> and <a class="el"
   * href="#SedErrorCategory_t">SedErrorCategory_t</a>,
   * respectively. @else set of constants.  If it @em
   * is one of the predefined error identifiers, the SedError class
   * assumes the error is a low-level system or Sed layer error and
   * <em>prepends</em> a built-in, predefined error message to any string
   * passed in the argument @p details to this constructor.  In addition,
   * all the predefined error identifiers have associated values for the
   * @p severity and @p category codes, and these fields are filled-in using
   * the libSed defaults for each different error identifier. @endif@~
   *
   * If the error identifier @p errorId is a number greater than 99999, the
   * SedError class assumes the error was generated from another part of
   * the software and does not do additional filling in of values beyond
   * the default in the constructor itself.  This allows SedError to serve
   * as a base class for other errors, such as for user-defined validation
   * rules (see Validator).  Callers should fill in all the parameters with
   * suitable values if generating errors with codes greater than 99999 to
   * make maximum use of the SedError facilities.
   *
   * @if clike As mentioned above, there are two other
   * enumerations, <a class="el"
   * href="#SedErrorSeverity_t">SedErrorSeverity_t</a> and <a class="el"
   * href="#SedErrorCategory_t">SedErrorCategory_t</a>, used for indicating
   * the severity and category of error for the predefined SedError codes.
   * The values passed in @p severity and @p category override the defaults
   * assigned based on the error code.  If the value of @p errorId is a
   * value from <a class="el" href="#SedErrorCode_t">SedErrorCode_t</a>,
   * callers do not need to fill in @p severity and @p category.
   * Conversely, if @p errorId is not a value from <a class="el"
   * href="#SedErrorCode_t">SedErrorCode_t</a>, callers can use other
   * values (not just those from <a class="el"
   * href="#SedErrorSeverity_t">SedErrorSeverity_t</a> and <a class="el"
   * href="#SedErrorCategory_t">SedErrorCategory_t</a>, but their own
   * special values) for @p severity and
   * @p category. @else As mentioned above,
   * there are additional constants defined for <a class="el"
   * href="#SedErrorSeverity_t">standard severity</a> and <a class="el"
   * href="#SedErrorCategory_t">standard category</a> codes, and every predefined
   * error in libSed has an associated value for severity and category taken
   * from these predefined sets.  These constants have symbol names
   * prefixed with <code>LIBSEDML_SEV_</code> and <code>LIBSEDML_CAT_</code>,
   * respectively.  If the value of @p errorId is one of the standard error
   * codes, callers do not need to fill in @p severity and @p category in a
   * call to this constructor.  Conversely, if @p errorId is not an existing
   * Sed-level error code, callers can use other values for @p severity and
   * @p category. @endif@~
   *
   * Please see the top of the documentation for SedError for a longer
   * discussion of the possible error codes, their meanings, and their
   * applicability to different combinations of Level+Version of Sed.
   *
   * @param errorId an unsigned int, the identification number of the error.
   *
   * @param level the Sed Level of the Sed model
   *
   * @param version the Sed Version within the Level of the Sed model
   *
   * @param details a string containing additional details about the error.
   * If the error code in @p errorId is one that is recognized by SedError,
   * the given message is @em appended to a predefined message associated
   * with the given code.  If the error code is not recognized, the message
   * is stored as-is as the text of the error.
   *
   * @param line an unsigned int, the line number at which the error occured.
   *
   * @param column an unsigned int, the column number at which the error occured.
   *
   * @param severity an integer indicating severity of the error.
   *
   * @param category an integer indicating the category to which the error
   * belongs.
   *
   * @param package the Sed Level package involved.
   *
   * @param pkgVersion the version of the @p package.
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  SedError
  (
    const unsigned int errorId  = 0
                                  , const unsigned int level    = SEDML_DEFAULT_LEVEL
                                      , const unsigned int version  = SEDML_DEFAULT_VERSION
                                          , const std::string& details  = ""
                                              , const unsigned int line     = 0
                                                  , const unsigned int column   = 0
                                                      , const unsigned int severity = LIBSEDML_SEV_ERROR
                                                          , const unsigned int category = LIBSEDML_CAT_SEDML
                                                              , const std::string& package  = "core"
                                                                  , const unsigned int pkgVersion = 1
  );


  /**
   * Copy constructor; creates a copy of this SedError.
   */
  SedError(const SedError& orig);


#ifndef SWIG

  /** @cond doxygen-libsbml-internal **/

  /**
   * clone function
   */
  virtual SedError* clone() const;

  /**
   * Outputs this SedError to stream in the following format (and followed
   * by a newline):
   *
   *   line: (error id) message
   *
   * @param stream the output stream to write to.
   */
  virtual void print(std::ostream& stream) const;

  /** @endcond **/

#endif  /* !SWIG */

protected:
  /** @cond doxygen-libsbml-internal **/

  virtual std::string stringForSeverity(unsigned int code) const;
  virtual std::string stringForCategory(unsigned int code) const;

  void adjustErrorId(unsigned int offset);

  /** @endcond **/
};

LIBSEDML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif /* SedError_h */
