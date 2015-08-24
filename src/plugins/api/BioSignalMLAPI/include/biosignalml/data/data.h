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

#ifndef BSML_DATA_H
#define BSML_DATA_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/recording.h>

#include <stdexcept>


namespace bsml {

  namespace data {

    class Exception : public std::runtime_error
    /*---------------------------------------*/
    {
     public:
      Exception(const std::string &msg) : std::runtime_error(msg) { }
      } ;

    class BIOSIGNALML_EXPORT Clock : public bsml::Clock
    /*-----------------------------------------------*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)
  
     public:
      virtual void extend(const double *times, const size_t length)
      {
        (void)times ;     // Unused parameters
        (void)length ;
        }
      } ;


    class BIOSIGNALML_EXPORT Signal : public bsml::Signal
    /*-------------------------------------------------*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)

     public:
      virtual void extend(const double *points, const size_t length)
      {
        (void)points ;    // Unused parameters
        (void)length ;
        }
      } ;


    template<class SIGNAL_TYPE = Signal>
    class BIOSIGNALML_EXPORT SignalArray : public std::vector<SIGNAL_TYPE *>
    /*--------------------------------------------------------------------*/
    {
      static_assert(std::is_base_of<Signal, SIGNAL_TYPE>::value, "SIGNAL_TYPE must be derived from Signal") ;

     public:
      virtual void extend(const double *points, const size_t length)
      {
        (void)points ;     // Unused parameters
        (void)length ;
        }
      virtual int index(const std::string &uri) const
      {
        (void)uri ;        // Unused parameter
        return -1 ;
        }
      } ;


    class BIOSIGNALML_EXPORT Recording : public bsml::Recording
    /*-------------------------------------------------------*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)

     public:
      template<class SIGNAL_TYPE=Signal, class CLOCK_TYPE=Clock>
      SignalArray<SIGNAL_TYPE> *new_signalarray(const std::vector<const std::string> &uris,
      /*---------------------------------------------------------------------------------*/
                                                const std::vector<const rdf::URI> &units,
                                                double rate)
      {
        return create_signalarray<SignalArray<SIGNAL_TYPE>, SIGNAL_TYPE, CLOCK_TYPE>(uris, units, rate, nullptr) ;
        }

      template<class SIGNAL_TYPE=Signal, class CLOCK_TYPE=Clock>
      SignalArray<SIGNAL_TYPE> *new_signalarray(const std::vector<const std::string> &uris,
      /*---------------------------------------------------------------------------------*/
                                                const std::vector<const rdf::URI> &units,
                                                CLOCK_TYPE *clock)
      {
        return create_signalarray<SignalArray<SIGNAL_TYPE>, SIGNAL_TYPE, CLOCK_TYPE>(uris, units, 0.0, clock) ;
        }


     protected:
      template<class SIGNAL_ARRAY_TYPE, class SIGNAL_TYPE, class CLOCK_TYPE>
      SIGNAL_ARRAY_TYPE *create_signalarray(const std::vector<const std::string> &uris,
      /*-----------------------------------------------------------------------------*/
                                            const std::vector<const rdf::URI> &units,
                                            double rate, CLOCK_TYPE *clock)
      {
        assert(uris.size() == units.size()) ;  // Lengths of `uris` and `units` are different
        auto sigs = new SIGNAL_ARRAY_TYPE() ;
        for (size_t n = 0 ;  n < uris.size() ;  ++n) {
          if (clock == nullptr) sigs->push_back(this->new_signal<SIGNAL_TYPE>(uris[n], units[n], rate)) ;
          else                  sigs->push_back(this->new_signal<SIGNAL_TYPE, CLOCK_TYPE>(uris[n], units[n], clock)) ;
          }
        return sigs ;
        }

      } ;

    } ;

  } ;


#endif
