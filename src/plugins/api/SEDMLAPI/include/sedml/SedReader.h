/**
 * @file    SedReader.h
 * @brief   Reads an Sed Document into memory
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
 * @class SedReader
 * @ingroup Core
 * @brief Methods for reading Sed from files and text strings.
 *
 * @htmlinclude not-sbml-warning.html
 *
 * The SedReader class provides the main interface for reading Sed
 * content from files and strings.  The methods for reading Sed all return
 * an SedDocument object representing the results.
 *
 * In the case of failures (such as if the Sed contains errors or a file
 * cannot be read), the errors will be recorded with the SedErrorLog
 * object kept in the SedDocument returned by SedReader.  Consequently,
 * immediately after calling a method on SedReader, callers should always
 * check for errors and warnings using the methods for this purpose
 * provided by SedDocument.
 *
 * For convenience as well as easy access from other languages besides C++,
 * this file also defines two global functions,
 * libsbml::readSedML(@if java String filename@endif)
 * and libsbml::readSedMLFromString(@if java String xml@endif).
 * They are equivalent to creating an SedReader
 * object and then calling the
 * SedReader::readSedML(@if java String filename@endif) or
 * SedReader::readSedMLFromString(@if java String xml@endif)
 * methods, respectively.
 *
 * @section compression Support for reading compressed files
 *
 * LibSed provides support for reading (as well as writing) compressed
 * Sed files.  The process is transparent to the calling
 * application&mdash;the application does not need to do anything
 * deliberate to invoke the functionality.  If a given Sed filename ends
 * with an extension for the @em gzip, @em zip or @em bzip2 compression
 * formats (respectively, @c .gz, @c .zip, or @c .bz2), then the methods
 * SedReader::readSedML(@if java String filename@endif) and
 * SedWriter::writeSed(@if java SedDocument d, String filename@endif)
 * will automatically decompress and compress the file while writing and
 * reading it.  If the filename has no such extension, it
 * will be read and written uncompressed as normal.
 *
 * The compression feature requires that the @em zlib (for @em gzip and @em
 * zip formats) and/or @em bzip2 (for @em bzip2 format) be available on the
 * system running libSed, and that libSed was configured with their
 * support compiled-in.  Please see the libSed @if clike <a href="libsbml-installation.html">installation instructions</a> @endif@if python <a href="libsbml-installation.html">installation instructions</a> @endif@if java  <a href="../../../libsbml-installation.html">installation instructions</a> @endif@~ for more information about this.  The methods
 * @if java SedReader::hasZlib()@else hasZlib()@endif@~ and
 * @if java SedReader::hasBzip2()@else hasBzip2()@endif@~
 * can be used by an application to query at run-time whether support
 * for the compression libraries is available in the present copy of
 * libSed.
 *
 * Support for compression is not mandated by the Sed standard, but
 * applications may find it helpful, particularly when large Sed models
 * are being communicated across data links of limited bandwidth.
 */

#ifndef SedReader_h
#define SedReader_h


#include <sedml/common/extern.h>
#include <sedml/common/sedmlfwd.h>
#include <sbml/util/util.h>


#ifdef __cplusplus


#include <string>

LIBSEDML_CPP_NAMESPACE_BEGIN

class SedDocument;


class LIBSEDML_EXTERN SedReader
{
public:

  /**
   * Creates a new SedReader and returns it.
   *
   * The libSed SedReader objects offer methods for reading Sed in
   * XML form from files and text strings.
   */
  SedReader();


  /**
   * Destroys this SedReader.
   */
  virtual ~SedReader();


  /**
   * Reads an Sed document from a file.
   *
   * This method is identical to SedReader::readSedMLFromFile(@if java String filename@endif).
   *
   * If the file named @p filename does not exist or its content is not
   * valid Sed, one or more errors will be logged with the SedDocument
   * object returned by this method.  Callers can use the methods on
   * SedDocument such as SedDocument::getNumErrors() and
   * SedDocument::getError(@if java long n@endif) to get the errors.  The object returned by
   * SedDocument::getError(@if java long n@endif) is an SedError object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for SedError for more information.
   *
   * If the file @p filename could not be read, the file-reading error will
   * appear first.  The error code @if clike (a value drawn from the enumeration
   * #XMLErrorCode_t) @endif@~ can provide a clue about what happened.  For example,
   * a file might be unreadable (either because it does not actually exist
   * or because the user does not have the necessary access priviledges to
   * read it) or some sort of file operation error may have been reported
   * by the underlying operating system.  Callers can check for these
   * situations using a program fragment such as the following:
   * @if clike
  @verbatim
  SedReader reader;
  SedDocument* doc  = reader.readSedMLFromFile(filename);

  if (doc->getNumErrors() > 0)
  {
   if (doc->getError(0)->getErrorId() == XMLError::XMLFileUnreadable)
   {
     // Handle case of unreadable file here.
   }
   else if (doc->getError(0)->getErrorId() == XMLError::XMLFileOperationError)
   {
     // Handle case of other file operation error here.
   }
   else
   {
     // Handle other cases -- see error codes defined in XMLErrorCode_t
     // for other possible cases to check.
   }
  }
  @endverbatim
  @endif@if java
  @verbatim
  SedReader reader = new SedReader();
  SedDocument doc  = reader.readSedMLFromFile(filename);

  if (doc.getNumErrors() > 0)
  {
     if (doc.getError(0).getErrorId() == libsbmlConstants.XMLFileUnreadable)
     {
         // Handle case of unreadable file here.
     }
     else if (doc.getError(0).getErrorId() == libsbmlConstants.XMLFileOperationError)
     {
         // Handle case of other file operation error here.
     }
     else
     {
         // Handle other error cases.
     }
  }
  @endverbatim
  @endif@if python
  @verbatim
  reader = SedReader()
  doc    = reader.readSedMLFromFile(filename)

  if doc.getNumErrors() > 0:
   if doc.getError(0).getErrorId() == libsbml.XMLFileUnreadable:
     # Handle case of unreadable file here.
   elif doc.getError(0).getErrorId() == libsbml.XMLFileOperationError:
     # Handle case of other file error here.
   else:
     # Handle other error cases here.

  @endverbatim
  @endif@if csharp
  @verbatim
  SedReader reader = new SedReader();
  SedDocument doc = reader.readSedMLFromFile(filename);

  if (doc.getNumErrors() > 0)
  {
     if (doc.getError(0).getErrorId() == libsbmlcs.libsbml.XMLFileUnreadable)
     {
          // Handle case of unreadable file here.
     }
     else if (doc.getError(0).getErrorId() == libsbmlcs.libsbml.XMLFileOperationError)
     {
          // Handle case of other file operation error here.
     }
     else
     {
          // Handle other cases -- see error codes defined in XMLErrorCode_t
          // for other possible cases to check.
     }
  }
  @endverbatim
  @endif@~
   *
   * If the given filename ends with the suffix @c ".gz" (for example, @c
   * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
   * file is assumed to be compressed in @em zip or @em bzip2 format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in @em zip format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   *
   * @htmlinclude note-reading-zipped-files.html
   *
   * @param filename the name or full pathname of the file to be read.
   *
   * @return a pointer to the SedDocument created from the Sed content.
   *
   * @note LibSed versions 2.x and later versions behave differently in
   * error handling in several respects.  One difference is how early some
   * errors are caught and whether libSed continues processing a file in
   * the face of some early errors.  In general, libSed versions after 2.x
   * stop parsing Sed inputs sooner than libSed version 2.x in the face
   * of XML errors, because the errors may invalidate any further Sed
   * content.  For example, a missing XML declaration at the beginning of
   * the file was ignored by libSed 2.x but in version 3.x and later, it
   * will cause libSed to stop parsing the rest of the input altogether.
   * While this behavior may seem more severe and intolerant, it was
   * necessary in order to provide uniform behavior regardless of which
   * underlying XML parser (Expat, Xerces, libxml2) is being used by
   * libSed.  The XML parsers themselves behave differently in their error
   * reporting, and sometimes libSed has to resort to the lowest common
   * denominator.
   *
   * @see SedError
   */
  SedDocument* readSedML(const std::string& filename);


  /**
   * Reads an Sed document from a file.
   *
   * This method is identical to SedReader::readSedML(@if java String filename@endif).
   *
   * If the file named @p filename does not exist or its content is not
   * valid Sed, one or more errors will be logged with the SedDocument
   * object returned by this method.  Callers can use the methods on
   * SedDocument such as SedDocument::getNumErrors() and
   * SedDocument::getError(@if java long n@endif) to get the errors.  The object returned by
   * SedDocument::getError(@if java long n@endif) is an SedError object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for SedError for more information.
   *
   * If the file @p filename could not be read, the file-reading error will
   * appear first.  The error code @if clike (a value drawn from the enumeration
   * #XMLErrorCode_t)@endif@~ can provide a clue about what happened.  For example,
   * a file might be unreadable (either because it does not actually exist
   * or because the user does not have the necessary access priviledges to
   * read it) or some sort of file operation error may have been reported
   * by the underlying operating system.  Callers can check for these
   * situations using a program fragment such as the following:
   * @if clike
  @verbatim
  SedReader* reader = new SedReader();
  SedDocument* doc  = reader.readSedML(filename);

  if (doc->getNumErrors() > 0)
  {
   if (doc->getError(0)->getErrorId() == XMLError::FileUnreadable)
   {
     // Handle case of unreadable file here.
   }
   else if (doc->getError(0)->getErrorId() == XMLError::FileOperationError)
   {
     // Handle case of other file operation error here.
   }
   else
   {
     // Handle other cases -- see error codes defined in XMLErrorCode_t
     // for other possible cases to check.
   }
  }
  @endverbatim
  @endif@if java
  @verbatim
  SedReader reader = new SedReader();
  SedDocument doc  = reader.readSedMLFromFile(filename);

  if (doc.getNumErrors() > 0)
  {
     if (doc.getError(0).getErrorId() == libsbmlConstants.XMLFileUnreadable)
     {
         // Handle case of unreadable file here.
     }
     else if (doc.getError(0).getErrorId() == libsbmlConstants.XMLFileOperationError)
     {
         // Handle case of other file operation error here.
     }
     else
     {
         // Handle other error cases.
     }
  }
  @endverbatim
  @endif@if python
  @verbatim
  reader = SedReader()
  doc    = reader.readSedMLFromFile(filename)

  if doc.getNumErrors() > 0:
   if doc.getError(0).getErrorId() == libsbml.XMLFileUnreadable:
     # Handle case of unreadable file here.
   elif doc.getError(0).getErrorId() == libsbml.XMLFileOperationError:
     # Handle case of other file error here.
   else:
     # Handle other error cases here.

  @endverbatim
  @endif@~
   *
   * If the given filename ends with the suffix @c ".gz" (for example, @c
   * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
   * file is assumed to be compressed in @em zip or @em bzip2 format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in @em zip format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   *
   * @htmlinclude note-reading-zipped-files.html
   *
   * @param filename the name or full pathname of the file to be read.
   *
   * @return a pointer to the SedDocument created from the Sed content.
   *
   * @note LibSed versions 2.x and later versions behave differently in
   * error handling in several respects.  One difference is how early some
   * errors are caught and whether libSed continues processing a file in
   * the face of some early errors.  In general, libSed versions after 2.x
   * stop parsing Sed inputs sooner than libSed version 2.x in the face
   * of XML errors, because the errors may invalidate any further Sed
   * content.  For example, a missing XML declaration at the beginning of
   * the file was ignored by libSed 2.x but in version 3.x and later, it
   * will cause libSed to stop parsing the rest of the input altogether.
   * While this behavior may seem more severe and intolerant, it was
   * necessary in order to provide uniform behavior regardless of which
   * underlying XML parser (Expat, Xerces, libxml2) is being used by
   * libSed.  The XML parsers themselves behave differently in their error
   * reporting, and sometimes libSed has to resort to the lowest common
   * denominator.
   *
   * @see SedError
   * @see SedDocument
   */
  SedDocument* readSedMLFromFile(const std::string& filename);


  /**
   * Reads an Sed document from the given XML string.
   *
   * This method is flexible with respect to the presence of an XML
   * declaration at the beginning of the string.  In particular, if the
   * string in @p xml does not begin with the XML declaration
   * <code>&lt;?xml version='1.0' encoding='UTF-8'?&gt;</code>, then this
   * method will automatically prepend the declaration to @p xml.
   *
   * This method will log a fatal error if the content given in the
   * parameter @p xml is not Sed.  See the method documentation for
   * SedReader::readSedML(@if java String filename@endif)
   * for an example of code for testing the returned error code.
   *
   * @param xml a string containing a full Sed model
   *
   * @return a pointer to the SedDocument created from the Sed content.
   *
   * @see SedReader::readSedML(@if java String filename@endif)
   */
  SedDocument* readSedMLFromString(const std::string& xml);


  /**
   * Static method; returns @c true if this copy of libSed supports
   * <i>gzip</I> and <i>zip</i> format compression.
   *
   * @return @c true if libSed has been linked with the <i>zlib</i>
   * library, @c false otherwise.
   *
   * @if notclike @note Because this is a @em static method, the
   * non-C++ language interfaces for libSed will contain two variants.  One
   * will be a static method on the class (i.e., SedReader), and the other
   * will be a standalone top-level function with the name
   * SedReader_hasZlib(). They are functionally identical. @endif@~
   *
   * @see @if clike hasBzip2() @else SedReader::hasBzip2() @endif@~
   */
  static bool hasZlib();


  /**
   * Static method; returns @c true if this copy of libSed supports
   * <i>bzip2</i> format compression.
   *
   * @return @c true if libSed is linked with the <i>bzip2</i>
   * libraries, @c false otherwise.
   *
   * @if notclike @note Because this is a @em static method, the
   * non-C++ language interfaces for libSed will contain two variants.  One
   * will be a static method on the class (i.e., SedReader), and the other
   * will be a standalone top-level function with the name
   * SedReader_hasBzip2(). They are functionally identical. @endif@~
   *
   * @see @if clike hasZlib() @else SedReader::hasZlib() @endif@~
   */
  static bool hasBzip2();


protected:
  /** @cond doxygen-libsbml-internal */

  /**
   * Used by readSedML() and readSedMLFromString().
   *
   * @if notcpp @htmlinclude warn-default-args-in-docs.html @endif@~
   */
  SedDocument* readInternal(const char* content, bool isFile = true);

  /** @endcond */
};

LIBSEDML_CPP_NAMESPACE_END

#endif /* __cplusplus */

LIBSEDML_CPP_NAMESPACE_BEGIN
BEGIN_C_DECLS


#ifndef SWIG


/**
 * Creates a new SedReader and returns it.  By default XML Schema
 * validation is off.
 */
LIBSEDML_EXTERN
SedReader_t *
SedReader_create(void);

/**
 * Frees the given SedReader.
 */
LIBSEDML_EXTERN
void
SedReader_free(SedReader_t *sr);


/**
 * Reads an Sed document from the given file.  If filename does not exist
 * or is not an Sed file, an error will be logged.  Errors can be
 * identified by their unique ids, e.g.:
 *
 * <code>
 *   SedReader_t   *sr;\n
 *   SedDocument_t *d;
 *
 *   sr = SedReader_create();
 *
 *   d = SedReader_readSedML(reader, filename);
 *
 *   if (SedDocument_getNumErrors(d) > 0)\n
 *   {\n
 *     if (XMLError_getId(SedDocument_getError(d, 0))
 *                                           == Sed_READ_ERROR_FILE_NOT_FOUND)\n
 *     if (XMLError_getId(SedDocument_getError(d, 0))
 *                                           == Sed_READ_ERROR_NOT_Sed)\n
 *   }\n
 * </code>
 *
 * If the given filename ends with the suffix @c ".gz" (for example, @c
 * "myfile.xml.gz"), the file is assumed to be compressed in @em gzip
 * format and will be automatically decompressed upon reading.
 * Similarly, if the given filename ends with @c ".zip" or @c ".bz2", the
 * file is assumed to be compressed in @em zip or @em bzip2 format
 * (respectively).  Files whose names lack these suffixes will be read
 * uncompressed.  Note that if the file is in @em zip format but the
 * archive contains more than one file, only the first file in the
 * archive will be read and the rest ignored.
 *
 * @note LibSed versions 2.x and 3.x behave differently in error
 * handling in several respects.  One difference is how early some errors
 * are caught and whether libSed continues processing a file in the face
 * of some early errors.  In general, libSed 3.x stops parsing Sed
 * inputs sooner than libSed 2.x in the face of XML errors because the
 * errors may invalidate any further Sed content.  For example, a
 * missing XML declaration at the beginning of the file was ignored by
 * libSed 2.x but in version 3.x, it will cause libSed to stop parsing
 * the rest of the input altogether.  While this behavior may seem more
 * severe and intolerant, it was necessary in order to provide uniform
 * behavior regardless of which underlying XML parser (Expat, Xerces,
 * libxml2) is being used by libSed.  The XML parsers themselves behave
 * differently in their error reporting, and sometimes libSed has to
 * resort to the lowest common denominator.
 *
 * @htmlinclude note-reading-zipped-files.html
 *
 * @return a pointer to the SedDocument read.
 */
LIBSEDML_EXTERN
SedDocument_t *
SedReader_readSedML(SedReader_t *sr, const char *filename);

LIBSEDML_EXTERN
SedDocument_t *
SedReader_readSedMLFromFile(SedReader_t *sr, const char *filename);

/**
 * Reads an Sed document from the given XML string.
 *
 * If the string does not begin with XML declaration:
 *
 *   <?xml version='1.0' encoding='UTF-8'?>
 *
 * it will be prepended.
 *
 * This method will log a fatal error if the XML string is not Sed.  See
 * the method documentation for readSedML(filename) for example error
 * checking code.
 *
 * @return a pointer to the SedDocument read.
 */
LIBSEDML_EXTERN
SedDocument_t *
SedReader_readSedMLFromString(SedReader_t *sr, const char *xml);


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * underlying libSed is linked with..
 *
 * @return @c non-zero if libSed is linked with zlib, @c zero otherwise.
 */
LIBSEDML_EXTERN
int
SedReader_hasZlib();


/**
 * Predicate returning @c non-zero or @c zero depending on whether
 * libSed is linked with bzip2.
 *
 * @return @c non-zero if libSed is linked with bzip2, @c zero otherwise.
 */
LIBSEDML_EXTERN
int
SedReader_hasBzip2();

#endif  /* !SWIG */


/**
 * Reads an Sed document from the given file @p filename.
 *
 * If @p filename does not exist, or it is not an Sed file, an error will
 * be logged in the error log of the SedDocument object returned by this
 * method.  Calling programs can inspect this error log to determine
 * the nature of the problem.  Please refer to the definition of
 * SedDocument for more information about the error reporting mechanism.
 *
 * @return a pointer to the SedDocument read.
 */
LIBSEDML_EXTERN
SedDocument_t *
readSedML(const char *filename);


LIBSEDML_EXTERN
SedDocument_t *
readSedMLFromFile(const char *filename);


/**
 * Reads an Sed document from a string assumed to be in XML format.
 *
 * If the string does not begin with XML declaration,
 *@verbatim
<?xml version='1.0' encoding='UTF-8'?>
@endverbatim
 *
 * an XML declaration string will be prepended.
 *
 * This method will report an error if the given string @p xml is not Sed.
 * The error will be logged in the error log of the SedDocument object
 * returned by this method.  Calling programs can inspect this error log to
 * determine the nature of the problem.  Please refer to the definition of
 * SedDocument for more information about the error reporting mechanism.
 *
 * @return a pointer to the SedDocument read.
 */
LIBSEDML_EXTERN
SedDocument_t *
readSedMLFromString(const char *xml);


END_C_DECLS
LIBSEDML_CPP_NAMESPACE_END

#endif  /* SedReader_h */
