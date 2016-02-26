/******************************************************************************
 *                                                                            *
 *  BioSignalML Management in C++                                             *
 *                                                                            *
 *  Copyright (c) 2010-2015  David Brooks                                     *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 ******************************************************************************/

#ifndef BSML_RESOURCE_H
#define BSML_RESOURCE_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/ontology.h>

#include <typedobject/typedobject.h>

#include <string>


namespace bsml {

  class BIOSIGNALML_EXPORT Resource : public tobj::TypedObject
  /*--------------------------------------------------------*/
  {
    TYPED_OBJECT(Resource, rdf::OWL::Object)

    // Generic attributes all resources have:
    // http://dublincore.org/documents/dc-rdf/
    PROPERTY_STRING(label, rdf::RDFS::label)  //!< A human-readable version of a resource's name.
                                         //!< The target must be a literal.
    PROPERTY_STRING(comment, rdf::RDFS::comment)  //!< A human-readable description of a resource.
    PROPERTY_STRING(description, rdf::DCT::description)  //!< An account of a resource's content.
    PROPERTY_NODE(precededBy, rdf::PRV::precededBy)
    PROPERTY_URI(creator, rdf::DCT::creator)
    PROPERTY_DATETIME(created, rdf::DCT::created)
    //, rdf::XSD.dateTime,  utils::datetime_to_isoformat, utils::isoformat_to_datetime)

    PREFIXES(BSML::NS, rdf::RDFS::NS, rdf::XSD::NS, rdf::DCT::NS, rdf::PRV::NS)
    } ;

  } ;

#endif
