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

#ifndef BSML_RECORDING_H
#define BSML_RECORDING_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/signal.h>
#include <biosignalml/event.h>
#include <biosignalml/annotation.h>
#include <biosignalml/segment.h>
#include <biosignalml/timing.h>
#include <biosignalml/resource.h>

#include <string>
#include <vector>
#include <cassert>
#include <list>


namespace bsml {

//  template<typename SIGNAL=bsml::Signal, typename CLOCK=bsml::Clock>
  class BIOSIGNALML_EXPORT Recording : public Resource
  /*------------------------------------------------*/
  {
    TYPED_OBJECT(Recording, BSML::Recording)

    PROPERTY_NODE(format, rdf::DCT::format)
    PROPERTY_NODE(dataset, BSML::dataset)
    PROPERTY_NODE_SET(source, rdf::DCT::source)

    PROPERTY_NODE(investigation, rdf::DCT::subject)
    PROPERTY_NODE(investigator, rdf::DCT::creator)  // Also Resource::creator
    PROPERTY_DATETIME(starttime, rdf::DCT::created) // Also Resource::created
    PROPERTY_DURATION(duration, rdf::DCT::extent)

    PROPERTY_OBJECT(timeline, rdf::TL::timeline, RelativeTimeLine)

//    PROPERTY_OBJECT(generatedBy, rdf::PROV::wasGeneratedBy, Provenace class...)
    PROPERTY_URI(generatedBy, rdf::PROV::wasGeneratedBy)   // TODO...

    // Other reources that directly refer to a Recording
      RESOURCE(BSML::recording,  Clock)    // The type of these must change to HDF5::Clock etc
      RESOURCE(BSML::recording,  Signal)
//    RESOURCE(BSML::recording,  Event)
//    RESOURCE(rdf::DCT::source, Segment)

// What we want are ALL bsml::Annotation resources in recording's Graph
//    RESOURCE(bsml::Annotation) // Is this enough to get C++ type??,    Annotation)
//    RESOURCE(rdf::DCT::subject,    Annotation)

   public:
    virtual void close(void) { }

    Interval::Ptr new_interval(const double start, const double duration, const std::string & units = "second") ;
    Instant::Ptr new_instant(const double start, const std::string & units = "second") ;

    template<typename CLOCK=Clock>
    std::list<rdf::URI> get_clock_uris(void)
    /*------------------------------------*/
    {
      return get_resource_uris<CLOCK>() ;
      }

    template<typename CLOCK=Clock>
    typename CLOCK::Ptr get_clock(const rdf::URI &uri)
    /*----------------------------------------------*/
    {
      return get_resource<CLOCK>(rdf::URI(uri)) ;
      }

    template<typename CLOCK=Clock>
    typename CLOCK::Ptr get_clock(const std::string &uri)
    /*-------------------------------------------------*/
    {
      return get_clock<CLOCK>(uri) ;
      }

    template<typename SIGNAL=Signal>
    std::list<rdf::URI> get_signal_uris(void)
    /*-------------------------------------*/
    {
      return get_resource_uris<SIGNAL>() ;
      }

    template<typename SIGNAL=Signal>
    typename SIGNAL::Ptr get_signal(const rdf::URI &uri)
    /*------------------------------------------------*/
    {
      return get_resource<SIGNAL>(uri) ;
      }

    template<typename SIGNAL=Signal>
    typename SIGNAL::Ptr get_signal(const std::string &uri)
    /*---------------------------------------------------*/
    {
      return get_signal<SIGNAL>(rdf::URI(uri)) ;
      }

    Event::Ptr get_event(const rdf::URI &uri) ;
    Event::Ptr get_event(const std::string &uri) ;
    std::list<rdf::URI> get_event_uris(const rdf::URI &type=rdf::URI()) ;

    Annotation::Ptr get_annotation(const rdf::URI &uri) ;
    Annotation::Ptr get_annotation(const std::string &uri) ;
    std::list<rdf::URI> get_annotation_uris(void) ;

    template<class CLOCK_TYPE=Clock>
    typename CLOCK_TYPE::Ptr new_clock(const std::string &uri, const rdf::URI &units)
    /*-----------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Clock, CLOCK_TYPE>::value, "CLOCK_TYPE must be derived from Clock") ;
      auto clock = CLOCK_TYPE::create(rdf::URI(uri, m_base), units) ;
      clock->set_recording(this->uri()) ;
      this->add_resource<CLOCK_TYPE>(clock) ;
      return clock ;
      }

    template<class SIGNAL_TYPE=Signal>
    typename SIGNAL_TYPE::Ptr new_signal(const std::string &uri, const rdf::URI &units, double rate)
    /*---------------------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(SIGNAL_TYPE::create(rdf::URI(uri, m_base), units, rate)) ;
      }

    template<class SIGNAL_TYPE=Signal, class CLOCK_TYPE=Clock>
    typename SIGNAL_TYPE::Ptr new_signal(const std::string &uri, const rdf::URI &units, typename CLOCK_TYPE::Ptr clock)
    /*---------------------------------------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(SIGNAL_TYPE::create(rdf::URI(uri, m_base), units, clock)) ;
      }

   protected:
    std::string m_base ;
    INITIALISE(                                                                 \
      m_base = this->uri().is_valid() ? (this->uri().to_string() + "/") : "" ;  \
      )

   private:
    template<class SIGNAL_TYPE>
    typename SIGNAL_TYPE::Ptr add_signal(typename SIGNAL_TYPE::Ptr signal)
    /*------------------------------------------------------------------*/
    {
      signal->set_recording(this->uri()) ;
      add_resource<SIGNAL_TYPE>(signal) ;
      return signal ;
      }

    } ;


  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
