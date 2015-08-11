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
#include <biosignalml/annotation.h>
#include <biosignalml/timing.h>
#include <biosignalml/object.h>

#include <string>
#include <vector>
#include <cassert>

using namespace rdf ;


namespace bsml {

  class BIOSIGNALML_EXPORT Recording : public Object
  /*----------------------------------------------*/
  {
    TYPED_OBJECT(Recording, BSML::Recording)

    PROPERTY_NODE(format, DCT::format)
    PROPERTY_NODE(dataset, BSML::dataset)
    PROPERTY_NODE_SET(source, DCT::source)

    PROPERTY_NODE(investigation, DCT::subject)
    PROPERTY_NODE(investigator, DCT::creator)  // Also Object::creator
    PROPERTY_DATETIME(starttime, DCT::created) // Also Object::created
    PROPERTY_DURATION(duration, DCT::extent)

    PROPERTY_OBJECT(timeline, TL::timeline, RelativeTimeLine)
    PROPERTY_NODE(generatedBy, PROV::wasGeneratedBy)  // SUBELEMENT/AOBJECT ??

    PROPERTY_OBJECT_RSET(clock_set, BSML::recording, Clock)
    PROPERTY_OBJECT_RSET(signal_set, BSML::recording, Signal)
    PROPERTY_OBJECT_RSET(annotation_set, DCT::subject, Annotation)

   public:
    template<class CLOCK_TYPE=Clock>
    CLOCK_TYPE *new_clock(const std::string &uri, const rdf::URI &units)
    /*----------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Clock, CLOCK_TYPE>::value, "CLOCK_TYPE must be derived from Clock") ;
      CLOCK_TYPE *clock = new CLOCK_TYPE(rdf::URI(uri, m_base), units) ;
      clock->set_recording(this->uri()) ;
      this->m_clock_set.insert(clock) ;
      return clock ;
      }

    template<class SIGNAL_TYPE=Signal>
    SIGNAL_TYPE *new_signal(const std::string &uri, const rdf::URI &units, double rate)
    /*-------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(new SIGNAL_TYPE(rdf::URI(uri, m_base), units, rate)) ;
      }

    template<class SIGNAL_TYPE=Signal, class CLOCK_TYPE=Clock>
    SIGNAL_TYPE *new_signal(const std::string &uri, const rdf::URI &units, CLOCK_TYPE *clock)
    /*----------------------------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;
      return add_signal<SIGNAL_TYPE>(new SIGNAL_TYPE(rdf::URI(uri, m_base), units, clock)) ;
      }

   protected:
    std::string m_base ;
    INITIALISE(                                 \
      m_base = this->uri().to_string() + "/" ;  \
      )

   private:
    template<class SIGNAL_TYPE>
    SIGNAL_TYPE *add_signal(SIGNAL_TYPE *signal)
    /*----------------------------------------*/
    {
      signal->set_recording(this->uri()) ;
      this->m_signal_set.insert(signal) ;
      return signal ;
      }

    } ;


  } ;


/**
##            'digest':        PropertyMap(BSML.digest),

  EventClass  = Event        #: The class of :class:`~.event.Event`\s in the Recording

**/

#endif
