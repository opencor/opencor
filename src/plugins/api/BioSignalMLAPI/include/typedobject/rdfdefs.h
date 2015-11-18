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

#ifndef TYPEDOBJECTS_RDFDEFS_H
#define TYPEDOBJECTS_RDFDEFS_H

#include <typedobject/typedobject_export.h>
#include <typedobject/rdf.h>


namespace rdf {

// Helper macros:
#define NAMESPACE(PREFIX, URI_PREFIX)                 \
  static const rdf::Namespace NS(PREFIX, URI_PREFIX) ;
#define TERM(NAME)                                    \
  static const rdf::URI NAME = NS.make_URI(#NAME) ;

  namespace RDF {
    NAMESPACE("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#")
    TERM(type)
    } ;

  namespace RDFS {
    NAMESPACE("rdfs", "http://www.w3.org/2000/01/rdf-schema#")
    TERM(label)
    TERM(comment)
    } ;

  namespace XSD {
    NAMESPACE("xsd", "http://www.w3.org/2001/XMLSchema#")
    TERM(dateTime)
    TERM(dayTimeDuration)
    } ;

  namespace OWL {
    NAMESPACE("owl", "http://www.w3.org/2002/07/owl#")
    TERM(Object)
    } ;

  namespace DCT {
    NAMESPACE("dct", "http://purl.org/dc/terms/")
    TERM(description)
    TERM(creator)
    TERM(created)
    TERM(format)
    TERM(source)
    TERM(subject)
    TERM(extent)
    } ;

  namespace PRV {
    NAMESPACE("prv", "http://purl.org/net/provenance/ns#")
    TERM(precededBy)
    } ;

  namespace PROV {
    NAMESPACE("prov", "http://www.w3.org/ns/prov#")
    TERM(wasGeneratedBy)
    } ;

  namespace TIME {
    NAMESPACE("time", "http://www.w3.org/2006/time#")
    TERM(TemporalEntity)
    } ;

  namespace TL {
    NAMESPACE("tl", "http://purl.org/NET/c4dm/timeline.owl#")
    TERM(RelativeTimeLine)
    TERM(timeline)
    TERM(at)
    TERM(start)
    TERM(duration)
    } ;

  } ;

#endif
