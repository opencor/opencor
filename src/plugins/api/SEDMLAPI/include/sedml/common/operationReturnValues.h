/**
 * @file    operationReturnValues.h
 * @brief   Enumeration of values returned by operations within libSEDML.
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
 */


#ifndef LIBSEDML_OPERATION_RETURN_VALUES_H
#define LIBSEDML_OPERATION_RETURN_VALUES_H

LIBSEDML_CPP_NAMESPACE_BEGIN

/**
 * Diagnostic return codes.
 *
 * Many methods in libSEDML return a status code to indicate whether the
 * operation requested by the caller succeeded or failed.  This enumeration
 * lists all the possible return codes from any libSEDML methods.
 */
typedef enum
{
  LIBSEDML_OPERATION_SUCCESS       = 0
                                     /*!< The operation was successful. */

  , LIBSEDML_INDEX_EXCEEDS_SIZE      = -1
                                       /*!< An index parameter exceeded the bounds of a data array or other
                                        * collection used in the operation.  This return value is typically
                                        * returned by methods that take index numbers to refer to lists
                                        * of objects, when the caller has provided an index that exceeds
                                        * the bounds of the list.  LibSEDML provides methods for checking the
                                        * size of list/sequence/collection structures, and callers should
                                        * verify the sizes before calling methods that take index numbers. */

  , LIBSEDML_UNEXPECTED_ATTRIBUTE    = -2
                                       /*!< The attribute that is the subject of this operation is not valid
                                        * for the combination of SEDML Level and Version for the underlying
                                        * object.  This can happen because libSEDML strives to offer a uniform
                                        * API for all SEDML Levels and Versions, but some object attributes and
                                        * elements are not defined for all SEDML Levels and Versions.  Calling
                                        * programs are expected to be aware of which object structures they
                                        * are working with, but when errors of this kind occur, they are
                                        * reported using this return value. */

  , LIBSEDML_OPERATION_FAILED        = -3
                                       /*!< The requested action could not be performed.  This can occur in
                                        * a variety of contexts, such as passing a null object as a parameter
                                        * in a situation where it does not make sense to permit a null object.
                                        */

  , LIBSEDML_INVALID_ATTRIBUTE_VALUE = -4
                                       /*!< A value passed as an argument to the method is not of a type that
                                        * is valid for the operation or kind of object involved.  For example,
                                        * this return code is used when a calling program attempts to set an
                                        * SEDML object identifier to a string whose syntax does not conform to
                                        * the SEDML identifier syntax. */

  , LIBSEDML_INVALID_OBJECT          = -5
                                       /*!< The object passed as an argument to the method is not of a type
                                        * that is valid for the operation or kind of object involved.  For
                                        * example, handing an invalidly-constructed ASTNode to a method
                                        * expecting an ASTNode will result in this error. */

  , LIBSEDML_DUPLICATE_OBJECT_ID     = -6
                                       /*!< There already exists an object with this identifier in the
                                        * context where this operation is being attempted.  This error is
                                        * typically returned in situations where SEDML object identifiers must be
                                        * unique, such as attempting to add two species with the same identifier
                                        * to a model. */

  , LIBSEDML_LEVEL_MISMATCH          = -7
                                       /*!< The SEDML Level associated with the object does not match the Level
                                        * of the parent object.  This error can happen when an SEDML component
                                        * such as a species or compartment object is created outside of a model
                                        * and a calling program then attempts to add the object to a model that
                                        * has a different SEDML Level defined. */

  , LIBSEDML_VERSION_MISMATCH        = -8
                                       /*!< The SEDML Version within the SEDML Level associated with the object
                                        * does not match the Version of the parent object.  This error can
                                        * happen when an SEDML component such as a species or compartment object
                                        * is created outside of a model and a calling program then attempts to
                                        * add the object to a model that has a different SEDML Level+Version
                                        * combination. */

  , LIBSEDML_INVALID_XML_OPERATION   = -9
                                       /*!< The XML operation attempted is not valid for the object or context
                                        * involved.  This error is typically returned by the XML interface layer
                                        * of libSEDML, when a calling program attempts to construct or manipulate
                                        * XML in an invalid way.  */

  , LIBSEDML_NAMESPACES_MISMATCH   = -10
                                     /*!< The SEDML Namespaces associated with the object
                                      * do not match the SEDML Namespaces of the parent object.  This error can
                                      * happen when an SEDML component such as a species or compartment object
                                      * is created outside of a model and a calling program then attempts to
                                      * add the object to a model that has a different SEDML Namespaces
                                      * combination. */

  , LIBSEDML_DUPLICATE_ANNOTATION_NS   = -11
                                         /*!< There already exists a top level annotation with the same namespace as
                                          * annoation being appended.  This error is
                                          * typically returned in situations where the appendAnnotation function
                                          * is being used to add an annotation that has a namespace that is already
                                          * present in the existing annotation. */

  , LIBSEDML_ANNOTATION_NAME_NOT_FOUND   = -12
      /*!< The existing annotation does not have a top-level element with
       * the given name. This error is
       * typically returned in situations where the
       * replaceTopLevelAnnotationElement function or
       * the removeTopLevelAnnotationElement function
       * is being used to replace or remove an annotation with a name that does
       * not match the name of any top-level element that is already
       * present in the existing annotation. */

  , LIBSEDML_ANNOTATION_NS_NOT_FOUND   = -13
                                         /*!< The existing annotation does not have a top-level element with
                                          * the given namespace. This error is
                                          * typically returned in situations where the
                                          * replaceTopLevelAnnotationElement function or
                                          * the removeTopLevelAnnotationElement function
                                          * is being used to replace or remove an annotation with a namespace that does
                                          * not match the namespace of any top-level element that is already
                                          * present in the existing annotation. */
                                         /* ---------------------------------------------------------------------------
                                          *
                                          *              Return codes for package extension
                                          *
                                          * -------------------------------------------------------------------------- */

  , LIBSEDML_PKG_VERSION_MISMATCH  = -20
                                     /*!< The Version of package extension within the SEDML Level and version
                                      * associated with the object does not match the Version of the parent
                                      * object. This error can happen when an SEDML component such as a layout
                                      * or group object is created outside of a model and a calling program
                                      * then attempts to add the object to a model that has a different SEDML
                                      * Level+Version+Package Version combination. */

  , LIBSEDML_PKG_UNKNOWN           = -21
                                     /*!< The required package extension is unknown. This error is typically
                                      * returned when creating an object of SBase derived class with the required
                                      * package, creating an object of SEDMLNamespaces or its derived class with the
                                      * required package, or invoking functions depending on the required package.
                                      * To avoid this error, the library of the required package needs to be
                                      * linked.
                                      */

  , LIBSEDML_PKG_UNKNOWN_VERSION    = -22
                                      /*!< The required version of the package extension is unknown. This error
                                       * is typically returned when creating an object of SBase derived class with
                                       * the required package, creating an object of SEDMLNamespaces or its derived
                                       * class with the required package, or invoking functions depending on the
                                       * required package. This error may be avoided by updating the library of the
                                       * required package to be linked.
                                       */

  , LIBSEDML_PKG_DISABLED            = -23
                                       /*!< The required package extension is disabled. This error is typically
                                        * returned when creating an object of SBase derived class with the required
                                        * package, creating an object of SEDMLNamespaces or its derived class with the
                                        * required package, or invoking functions depending on the required  package.
                                        * To avoid this error, the library of the required package needs to be enabled.
                                        */

  , LIBSEDML_PKG_CONFLICTED_VERSION  = -24
                                       /*!< Another version of the required package extension has already been enabled
                                        * in the target SBase object, or enabled in the model to/in which the target
                                        * object to be added/contained. This error is typically returned  when adding an
                                        * object of some SBase derived class with the required package to other SBase
                                        * derived object, or when enabling the required package in the target object.
                                        * To avoid this error, the conflict of versions need to be avoided.
                                        */

  , LIBSEDML_PKG_CONFLICT            = -25
                                       /*!< Another SEDML package extension for the same URI has already been registered.
                                        * This error is typically returned when adding a SEDML package extension to the
                                        * SEDMLExtensionRegistry. To avoid this error, ensure that SEDML package
                                        * extensions are only registered once.
                                        */

                                       /* ---------------------------------------------------------------------------
                                        *
                                        *              Return codes for conversion extensions
                                        *
                                        * -------------------------------------------------------------------------- */

  , LIBSEDML_CONV_INVALID_TARGET_NAMESPACE  = -30
      /*!< The target namespace is not a valid SEDML namespace. while
       * attempting to convert the SEDML document using
       * SEDMLLevelVersionConverter::convert() or related methods, the target
       * namespace has been found to be invalid or unset.  (The function
       * {@link SEDMLNamespaces#isValidCombination()} may be useful in
       * detecting this situation and preventing the error.)
       */

  , LIBSEDML_CONV_PKG_CONVERSION_NOT_AVAILABLE = -31
      /*!< Conversions involving packages are not available in the specified
       * routine. This error is typically returned when calling a converter
       * that does not have the functionality to deal with SEDML Level&nbsp;3
       * packages. To avoid this error, ensure that the requested
       * ConversionProperties specifies packages.
       */

  , LIBSEDML_CONV_INVALID_SRC_DOCUMENT = -32
                                         /*!< The document on which conversion is being requested is invalid and
                                          * the requested conversion cannot be performed. This error is
                                          * typically returned when a conversion routine has been given an
                                          * invalid target document or the conversion requires a certain degree
                                          * of validity that is not present in the document. To avoid this
                                          * error, use the SEDMLDocument::checkConsistency() function to find and
                                          * resolve errors before passing the document to a conversion method.
                                          */

  , LIBSEDML_CONV_CONVERSION_NOT_AVAILABLE = -33
      /*!< Conversion with the given properties is not yet available.
       */

  , LIBSEDML_CONV_PKG_CONSIDERED_UNKNOWN = -34
      /*!< The package that is being stripped is not an enabled
       * package but considered by libSEDML to be an unrecognized
       * package.  This error is typically returned when calling
       * the StripPackage converter requesting that a package
       * for which code is not available be stripped. Thus the
       * containing document will not be altered as the elements
       * and attributes for this package are stored as unknown package
       * information and will be written out as such.
       */

} SedOperationReturnValues_t;

LIBSEDML_CPP_NAMESPACE_END


#endif  /* LIBSEDML_OPERATION_RETURN_VALUES_H */

