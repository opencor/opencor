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

  namespace RDF {
    static const Namespace NS("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#") ;
    static const URI type = NS.make_URI("type") ;
    } ;

  namespace RDFS {
    const Namespace NS("rdfs", "http://www.w3.org/2000/01/rdf-schema#") ;
    const URI label = NS.make_URI("label") ;
    const URI comment = NS.make_URI("comment") ;
    } ;

  namespace XSD {
    const Namespace NS("xsd", "http://www.w3.org/2001/XMLSchema#") ;
    const URI dateTime = NS.make_URI("dateTime") ;
    const URI dayTimeDuration = NS.make_URI("dayTimeDuration") ;
    } ;

  namespace OWL {
    const Namespace NS("owl", "http://www.w3.org/2002/07/owl#") ;
    const URI Object = NS.make_URI("Object") ;
    } ;

  namespace DCT {
    const Namespace NS("dct", "http://purl.org/dc/terms/") ;
    const URI description = NS.make_URI("description") ;
    const URI creator = NS.make_URI("creator") ;
    const URI created = NS.make_URI("created") ;
    const URI format = NS.make_URI("format") ;
    const URI source = NS.make_URI("source") ;
    const URI subject = NS.make_URI("subject") ;
    const URI extent = NS.make_URI("extent") ;
    } ;

  namespace PRV {
    const Namespace NS("prv", "http://purl.org/net/provenance/ns#") ;
    const URI precededBy = NS.make_URI("precededBy") ;
    } ;

  namespace PROV {
    const Namespace NS("prov", "http://www.w3.org/ns/prov#") ;
    const URI wasGeneratedBy = NS.make_URI("generaratedBy") ;
    } ;

  namespace TIME {
    const Namespace NS("time", "http://www.w3.org/2006/time#") ;
    const URI TemporalEntity = NS.make_URI("TemporalEntity") ;
    } ;

  namespace TL {
    const Namespace NS("tl", "http://purl.org/NET/c4dm/timeline.owl#") ;
    const URI RelativeTimeLine = NS.make_URI("RelativeTimeLine") ;
    const URI timeline = NS.make_URI("timeline") ;
    const URI at = NS.make_URI("at") ;
    const URI start = NS.make_URI("start") ;
    const URI duration = NS.make_URI("duration") ;
    } ;

  } ;

#endif
