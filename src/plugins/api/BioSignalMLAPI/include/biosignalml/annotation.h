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
#include <biosignalml/object.h>
#include <biosignalml/timing.h>

using namespace rdf ;


namespace bsml {

  class BIOSIGNALML_EXPORT Annotation : public Object
  /*-----------------------------------------------*/
  {
    TYPED_OBJECT(Annotation, BSML::Annotation)

    PROPERTY_URI(about, DCT::subject)
    PROPERTY_NODE_SET(tags, BSML::tag)
    PROPERTY_OBJECT(time, BSML::time, TemporalEntity)
    } ;

  } ;

#endif
