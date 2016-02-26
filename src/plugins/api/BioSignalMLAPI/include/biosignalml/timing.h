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
#include <biosignalml/resource.h>

#include <string>
#include <vector>


namespace bsml {

  class Interval ;
  class Instant ;

  class BIOSIGNALML_EXPORT RelativeTimeLine : public Resource
  /*-------------------------------------------------------*/
  {
    TYPED_OBJECT(RelativeTimeLine, rdf::TL::RelativeTimeLine)
    } ;


  class BIOSIGNALML_EXPORT TemporalEntity : public Resource
  /*-----------------------------------------------------*/
  {
    TYPED_OBJECT(TemporalEntity, rdf::TIME::TemporalEntity)

    PROPERTY_OBJECT(timeline, rdf::TL::timeline, RelativeTimeLine)
    PROPERTY_DURATION(start, NONE)
    PROPERTY_DURATION(duration, NONE)
    } ;


  //! An interval on a time line.
  //!
  //! Intervals are `closed` in the mathematical sense, that is they contain
  //! their end points.
  class BIOSIGNALML_EXPORT Interval : public TemporalEntity
  /*-----------------------------------------------------*/
  {
    TYPED_OBJECT(Interval, BSML::Interval)

    ASSIGN_DURATION(start, rdf::TL::start)
    ASSIGN_DURATION(duration, rdf::TL::duration)

   public:
    Interval(const rdf::URI &uri, const double start, const double duration,
             const std::string & units = "second", RelativeTimeLine::Ptr timeline=nullptr) ;
    } ;


  class BIOSIGNALML_EXPORT Instant : public TemporalEntity
  /*----------------------------------------------------*/
  {
    TYPED_OBJECT(Instant, BSML::Instant)

    ASSIGN_DURATION(start, rdf::TL::at)

   public:
    Instant(const rdf::URI &uri, const double start, const std::string & units = "second",
            RelativeTimeLine::Ptr timeline=nullptr) ;

    } ;


  class BIOSIGNALML_EXPORT Clock : public Resource
  /*--------------------------------------------*/
  {
    TYPED_OBJECT(Clock, BSML::SampleClock)

    // Also have 'frequency' ?? and/or 'period' ??
    PROPERTY_URI(units,          BSML::units)
    PROPERTY_DECIMAL(resolution, BSML::resolution)
    PROPERTY_DECIMAL(rate,       BSML::rate)
    PROPERTY_URI(recording,      BSML::recording)

   public:
    Clock(const rdf::URI &uri, const rdf::URI &units) ;

    virtual double time(const size_t n) const ;
    //! Return the position of the first time point that is
    //! not less than `t`. i.e. largest `n` such that `time(n) <= t`.
    virtual size_t index(const double t) const ;
    virtual size_t index_right(const double t) const ;
    virtual void extend(const double *times, const size_t length) ;
    virtual std::vector<double> read(size_t pos=0, intmax_t length=-1) ;
    void extend(const std::vector<double> &points) ;
    } ;

  } ;

#endif
