/*****************************************************************************
 *                                                                           *
 *  Typed RDF Objects for C++                                                *
 *                                                                           *
 *  Copyright (c) 2014-2015  David Brooks                                    *
 *                                                                           *
 *  Licensed under the Apache License, Version 2.0 (the "License");          *
 *  you may not use this file except in compliance with the License.         *
 *  You may obtain a copy of the License at                                  *
 *                                                                           *
 *      http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                           *
 *  Unless required by applicable law or agreed to in writing, software      *
 *  distributed under the License is distributed on an "AS IS" BASIS,        *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 *  See the License for the specific language governing permissions and      *
 *  limitations under the License.                                           *
 *                                                                           *
 *****************************************************************************/

#ifndef TYPEDOBJECT_RDF_H
#define TYPEDOBJECT_RDF_H

#include <typedobject/typedobject_export.h>
#include <typedobject/common.h>
#include <typedobject/xsd.h>

//**************************************************************************//

#include <string>
#include <set>
#include <cmath>
#include <cinttypes>
#include <iostream>
#include <functional>
#include <memory>
#include <unordered_map>

//**************************************************************************//

namespace tobj {
  class TypedObject ;                // Declare forward
  } ;

//**************************************************************************//

namespace rdf {

  class NodeImpl ;                   // Declare forward
  class StmntImpl ;                  // Declare forward
  class IterImpl ;                   // Declare forward
  class GraphImpl ;                  // Declare forward

  class Graph ;                      // Declare forward for VS2013

  class TYPEDOBJECT_EXPORT Node
  /*-------------------------*/
  {
   public:
    Node() ;
    Node(NodeImpl * node) ;
    Node(const Node & other) ;
    Node(Node && other) ;
    virtual ~Node() ;
    Node & operator=(const Node & other) ;
    Node & operator=(Node && other) ;

    NodeImpl *node(void) const ;

    bool operator==(const Node &other) const ;
    bool operator<(const Node &other) const ;
    explicit operator std::string() const ;
    friend TYPEDOBJECT_EXPORT std::ostream & operator<<(std::ostream & os, const Node & node) ;

    bool is_valid(void) const ;
    bool is_literal_type(const char *type_uri) const ;
    bool is_uri(void) const ;
    bool is_blank(void) const ;
    bool is_int(void) const ;
    bool is_float(void) const ;
    bool is_bool(void) const ;

    std::string to_string(void) const ;
    int to_int(void) const ;
    float to_float(void) const ;
    bool to_bool(void) const  ;

   private:
    NodeImpl *m_node ;
    friend class NodeImpl ;
    } ;

  TYPEDOBJECT_EXPORT std::ostream & operator<<(std::ostream & os, const Node & node) ;


  class TYPEDOBJECT_EXPORT URI : public Node
  /*--------------------------------------*/
  {
   public:
    URI() ;
    URI(const std::string & uri) ;
    URI(const std::string & uri, const std::string & base) ;
    URI(const Node & uri) ;
    URI(const URI & other) ;
    URI(URI && other) ;

    URI & operator=(const URI & other) ;
    URI & operator=(URI && other) ;

    /**
     * Generate a unique URI that starts with this URI.
     *
     * :param sibling: When set, replace the last component of our URI with unique text.
     *   The default is to append unique text to our URI.
     * :type: bool
     * :param prefix: If set, insert between the URI and unique text.
     * :type: str
     * :return: A unique URI.
     * :rtype: Uri
     */
    URI make_URI(bool sibling=false, const std::string &prefix="") const ;
    } ;


  class TYPEDOBJECT_EXPORT BNode : public Node
  /*----------------------------------------*/
  {
   public:
    BNode() ;
    BNode(const std::string & s) ;
    BNode(const BNode & other) ;
    BNode(BNode && other) ;
    BNode & operator=(const BNode & other) ;
    BNode & operator=(BNode && other) ;
    } ;


  class TYPEDOBJECT_EXPORT Literal : public Node
  /*------------------------------------------*/
  {
   public:
    Literal() ;
    Literal(const std::string & s) ;
    Literal(const std::string & s, const rdf::URI & datatype) ;
    Literal(const std::string & s, const std::string & language) ;
    Literal(xsd::Decimal d, unsigned frac_digits=7) ;
    Literal(xsd::Integer i) ;
    Literal(const Literal & other) ;
    Literal(Literal && other) ;
    Literal & operator=(const Literal & other) ;
    Literal & operator=(Literal && other) ;

    class TYPEDOBJECT_EXPORT Constants {
     public:
      static const char * EMPTY_STRING ;
      static const xsd::Decimal EMPTY_DECIMAL ;
      static const xsd::Integer EMPTY_INTEGER ;
      } ;

    inline static bool not_empty(const std::string &s) { return (s != Constants::EMPTY_STRING) ; }
    inline static bool not_empty(const xsd::Decimal d) { return std::isfinite(d) ; }
    inline static bool not_empty(const xsd::Integer i) { return (i != Constants::EMPTY_INTEGER) ; }
    } ;


  class TYPEDOBJECT_EXPORT Namespace
  /*------------------------------*/
  {
   public:
    Namespace(const std::string & name, const std::string & uri) ;
    Namespace(const Namespace & other) ;
    Namespace & operator=(const Namespace & other) ;

    bool operator==(const Namespace &other) const ;
    bool operator<(const Namespace &other) const ;

    const URI make_URI(const std::string &suffix) const ;

    Literal name(void) const ;
    URI uri(void) const ;

   private:
    const Literal m_name ;
    const URI m_uri ;
    } ;


  class TYPEDOBJECT_EXPORT Statement
  /*------------------------------*/
  {
   public:
    Statement() ;
    Statement(const Node & s, const Node & p, const Node & o) ;
    Statement(const Statement & other) ;
    Statement(Statement && other) ;
    Statement(StmntImpl *stmnt) ;
    virtual ~Statement() ;
    Statement & operator=(const Statement & other) ;
    Statement & operator=(Statement && other) ;

   private:
    StmntImpl *m_stmnt ;
    friend class Graph ;
    } ;


  class TYPEDOBJECT_EXPORT StatementIter
  /*----------------------------------*/
  {
   public:
    StatementIter() ;
    StatementIter(IterImpl * iter) ;
    virtual ~StatementIter() ;

    bool end(void) const ;
    bool next(void) const ;
    StatementIter & operator++ (void) ;
    Statement get_statement(void) const ;
    const Node get_subject(void) const ;
    const Node get_predicate(void) const ;
    const Node get_object(void) const ;

   private:
    IterImpl *m_iter ;
    friend class Graph ;
    } ;

  } ;

//**************************************************************************//

namespace std {

  template <> struct hash<rdf::URI>     // Specify now since referenced below
  /*------------------------------*/
  {
    size_t operator()(const rdf::URI & uri) const
    /*-----------------------------------------*/
    {
      return hash<std::string>()(uri.to_string()) ;
      }

    } ;

  } ;

//**************************************************************************//

namespace rdf {

  class TYPEDOBJECT_EXPORT Graph
  /*--------------------------*/
  {
    SHARED_PTR(Graph)

   public:
    Graph() ;
    Graph(const rdf::URI & uri) ;
    virtual ~Graph() ;

    enum class Format {
      UNKNOWN = 0,
      RDFXML,
      TURTLE,
      NTRIPLES,
      JSON
      } ;

    static const std::string format_to_mimetype(Format format) ;
    static Format mimetype_to_format(const std::string &mimetype) ;

    static Ptr create_from_resource(const std::string &resource,
                                    const Format format=Format::RDFXML,
                                    const std::string &base="") ;
    static Ptr create_from_string(const rdf::URI &uri,
                                  const std::string &source,
                                  const Format format=Format::RDFXML,
                                  const std::string &base="") ;

    void parse_resource(const std::string &resource,
                        const Format format=Format::RDFXML,
                        const std::string &base="") ;
    void parse_string(const std::string &source,
                      const Format format=Format::RDFXML,
                      const std::string &base="") ;

    std::string serialise(const Format format=Format::RDFXML,
                          const std::string &base="",
                          const std::set<Namespace> &prefixes=std::set<Namespace>()) ;

    void add_prefixes(const std::set<Namespace> &prefixes) ;
    const URI &uri(void) const ;

    bool insert(const Statement &statement) const ;
    bool insert(const rdf::Node &s, const rdf::Node &p, const rdf::Node &o) const ;
    void add_statements(const StatementIter &statements) const ;
    bool contains(const Statement &p_statement) const ;
    bool contains(const Node &s, const Node &p, const Node &o) const ;
    StatementIter get_statements(const Statement &pattern) const ;
    StatementIter get_statements(const Node &s, const Node &p, const Node &o) const ;

    std::shared_ptr<tobj::TypedObject> get_resource(const URI &uri) ;
    void add_resource(const URI &uri, std::weak_ptr<tobj::TypedObject> weakref) ;
    void delete_resource(const URI &uri) ;

   private:
    const URI m_uri ;
    GraphImpl *m_graph ;
    // Can't forward declare `tobj::TypedObject::Registry`
    std::unordered_map<URI, std::weak_ptr<tobj::TypedObject>> m_objectregistry ;
    } ;

  } ;

//**************************************************************************//

#endif
