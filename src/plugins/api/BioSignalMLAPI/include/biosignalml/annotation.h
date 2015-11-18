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

#ifndef BSML_ANNOTATION_H
#define BSML_ANNOTATION_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/resource.h>
#include <biosignalml/timing.h>

using namespace rdf ;


namespace bsml {

  class BIOSIGNALML_EXPORT Annotation : public Resource
  /*-------------------------------------------------*/
  {
    TYPED_OBJECT(Annotation, BSML::Annotation)

    PROPERTY_OBJECT(about, DCT::subject, Resource)
    PROPERTY_NODE_SET(tags, BSML::tag)
    PROPERTY_OBJECT(annotation_time, BSML::time, TemporalEntity)

   public:
    Annotation(const rdf::URI &uri, Resource::Ptr about, const std::string &text="",
               const std::set<rdf::Node> &tags=std::set<rdf::Node>(),
               const Annotation::Ptr &predecessor=nullptr) ;

    //!< The time, if any, associated with the resource that is annotated.
    TemporalEntity::Ptr time(void) const ;
    } ;

  } ;

#endif
