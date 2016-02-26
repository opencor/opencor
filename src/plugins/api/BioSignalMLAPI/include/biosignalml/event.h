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

#ifndef BSML_EVENT_H
#define BSML_EVENT_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/resource.h>
#include <biosignalml/timing.h>


namespace bsml {

  class BIOSIGNALML_EXPORT Event : public Resource
  /*--------------------------------------------*/
  {
    TYPED_OBJECT(Event, BSML::Event)

    PROPERTY_URI(recording, BSML::recording)
    PROPERTY_URI(eventtype, BSML::eventType)
    PROPERTY_OBJECT(time, BSML::time, TemporalEntity) // Instant or Interval
    } ;

  } ;

#endif
