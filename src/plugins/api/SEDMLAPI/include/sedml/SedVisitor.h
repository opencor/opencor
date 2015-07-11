/**
 * @file   SedVisitor.h
 * @brief  Visitor Design Pattern for the Sed object tree
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
 * @class SedVisitor
 * @ingroup Core
 * @brief Support class for performing operations on Sed objects.
 *
 * <em style='color: #555'>This class of objects is defined by libSed only
 * and has no direct equivalent in terms of Sed components.</em>
 *
 * This is a class that supports the use of the <a target="_blank"
 * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
 * Pattern</i></a> to perform operations on Sed objects.  The LibSed
 * implementation of the Visitor Pattern couples the notion of visitation
 * with the traversal of an Sed model oject hierarchy.
 *
 * This class (SedVisitor) does not provide stand-alone functionality;
 * rather, it defines the interface that user visitor classes must
 * implement.  All of the individual Sed object classes have methods named
 * <code>accept</code> that are used for invoking an object of class
 * SedVisitor.  An example of its use is in the Sed validation system,
 * which is internally implemented using this Visitor Pattern facility.
 */

#ifndef SedVisitor_h
#define SedVisitor_h


#ifdef __cplusplus


#include <sedml/SedTypeCodes.h>

LIBSEDML_CPP_NAMESPACE_BEGIN

/**
 * Forward class name declarations avoid cyclic dependencies.
 */

class SedBase;

class SedDocument;
class SedListOf;


class SedVisitor
{
public:

  /**
   * Destructor method.
   */
  virtual ~SedVisitor();

  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SedDocument objects.
   *
   * @param x the SedDocument object to visit.
   */
  virtual void visit(const SedDocument &x);

  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ListOf objects.
   *
   * @param x the ListOf object to visit.
   *
   * @param type the object type code.
   */
  virtual void visit(const SedListOf       &x, int type);


  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SedBase objects.
   *
   * @param x the SedBase object to visit.
   */
  virtual bool visit(const SedBase                    &x);

  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on SedDocument objects.
   *
   * @param x the SedDocument object to visit.
   */
  virtual void leave(const SedDocument &x);

  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on Reaction objects.
   *
   * @param x the Reaction object to visit.
   */
  virtual void leave(const SedBase     &x);


  /**
   * Interface method for using the <a target="_blank"
   * href="http://en.wikipedia.org/wiki/Design_pattern_(computer_science)"><i>Visitor
   * Pattern</i></a> to perform operations on ListOf objects.
   *
   * @param x the ListOf object to visit.
   *
   * @param type the object type code.
   *
   */
  virtual void leave(const SedListOf &x, int type);
};

LIBSEDML_CPP_NAMESPACE_END

#endif  /* __cplusplus */
#endif  /* SedVisitor_h */

