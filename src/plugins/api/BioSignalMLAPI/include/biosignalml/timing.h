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

#ifndef BSML_TIMING_H
#define BSML_TIMING_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/timing.h>
#include <biosignalml/object.h>

#include <string>

using namespace rdf ;

namespace bsml {

  class BIOSIGNALML_EXPORT RelativeTimeLine : public Object
  /*-----------------------------------------------------*/
  {
    TYPED_OBJECT(RelativeTimeLine, TL::RelativeTimeLine)
    } ;


  class BIOSIGNALML_EXPORT TemporalEntity : public Object
  /*---------------------------------------------------*/
  {
    TYPED_OBJECT(TemporalEntity, TIME::TemporalEntity)

    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
    PROPERTY_DURATION(start, NONE)
    PROPERTY_DURATION(duration, NONE)
    } ;


  class BIOSIGNALML_EXPORT Interval : public TemporalEntity
  /*-----------------------------------------------------*/
  {
    TYPED_OBJECT(Interval, BSML::Interval)

    ASSIGN_DURATION(start, TL::start)
    ASSIGN_DURATION(duration, TL::duration)
    } ;


  class BIOSIGNALML_EXPORT Instant : public TemporalEntity
  /*----------------------------------------------------*/
  {
    TYPED_OBJECT(Instant, BSML::Instant)

    ASSIGN_DURATION(start, TL::at)
    } ;


  class BIOSIGNALML_EXPORT Clock : public Object
  /*------------------------------------------*/
  {
    TYPED_OBJECT(Clock, BSML::SampleClock)

    // Also have 'frequency' ?? and/or 'period' ??
    PROPERTY_URI(units,          BSML::units)
    PROPERTY_DECIMAL(resolution, BSML::resolution)
    PROPERTY_DECIMAL(rate,       BSML::rate)
    PROPERTY_URI(recording,      BSML::recording)

   public:
    Clock(const rdf::URI &uri, const rdf::URI &units) ;

    } ;

  } ;

#endif
