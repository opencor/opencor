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

#ifndef BSML_HDF5_H
#define BSML_HDF5_H

#include <biosignalml/biosignalml_export.h>
#include <biosignalml/data/data.h>
#include <biosignalml/biosignalml.h>

#include <string>
#include <memory>
#include <list>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

namespace bsml {

  namespace HDF5 {

    const std::string BSML_H5_MAJOR   = "1" ;
    const std::string BSML_H5_MINOR   = "0" ;
    const std::string BSML_H5_VERSION = std::string("BSML ") + BSML_H5_MAJOR + "." + BSML_H5_MINOR ;

    enum H5Compression {
      BSML_H5_COMPRESS_NONE,
      BSML_H5_COMPRESS_GZIP,
      BSML_H5_COMPRESS_SZIP
      } ;


    class File ;        // Declare forward
    class Dataset ;     // Declare forward
    class ClockData ;   // Declare forward
    class SignalData ;  // Declare forward

    class Recording ;   // VS2013 needs class visible for friendship...
    class Signal ;      // VS2013 needs class visible for friendship...

    class IOError : public data::Exception
    /*----------------------------------*/
    {
     public:
      IOError(const std::string &msg) : bsml::data::Exception(msg) { }
      } ;

    class Exception : public data::Exception
    /*------------------------------------*/
    {
     public:
      Exception(const std::string &msg) : bsml::data::Exception(msg) { }
      } ;


    class BIOSIGNALML_EXPORT Clock : public bsml::Clock
    /*-----------------------------------------------*/
    {
      TYPED_OBJECT(Clock, BSML::SampleClock)

     public:
      Clock(const rdf::URI &uri, const rdf::URI &units) ;
      double time(const size_t n) const override ;
      //! Return the position of the first time point that is
      //! not less than `t`.
      size_t index(const double t) const override ;
      size_t index_right(const double t) const override ;
      void extend(const double *times, const size_t length) override ;
      std::vector<double> read(size_t pos=0, ssize_t length=-1) override ;

     private:
      std::shared_ptr<ClockData> m_data ;
      friend class Signal ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT Signal : public bsml::Signal
    /*-------------------------------------------------*/
    {
      TYPED_OBJECT(Signal, BSML::Signal)
      PROPERTY_OBJECT(clock, BSML::clock, Clock)                     // Override class

     public:
      Signal(const rdf::URI &uri, const rdf::URI &units, double rate) ;
      Signal(const rdf::URI &uri, const rdf::URI &units, Clock::Ptr clock) ;
      void extend(const double *points, const size_t length) override ;
      //! Read all points spanned by the closed interval (i.e. include points at start
      //! and end of interval.
      data::TimeSeries::Ptr read(Interval::Ptr interval, ssize_t maxpoints=-1) override ;
      data::TimeSeries::Ptr read(size_t pos=0, ssize_t length=-1) override ;

     private:
      std::shared_ptr<SignalData> m_data ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT SignalArray : public data::SignalArray<HDF5::Signal>
    /*-------------------------------------------------------------------------*/
    {
     public:
      typedef std::shared_ptr<SignalArray> Ptr ;

      template<typename... Args>
      inline static Ptr new_reference(Args... args)
      {
        return std::make_shared<SignalArray>(args...) ;
        }

      void extend(const double *points, const size_t length) ;
      int index(const std::string &uri) const ;

     private:
      std::shared_ptr<SignalData> m_data ;
      friend class Recording ;
      } ;


    class BIOSIGNALML_EXPORT Recording : public data::Recording
    /*-------------------------------------------------------*/
    {
      TYPED_OBJECT(Recording, BSML::Recording)
      RESTRICT_NODE(format, Format::HDF5)

      RESOURCE(BSML::recording, Clock)    // The type of these must change to HDF5::Clock etc
      RESOURCE(BSML::recording, Signal)

     public:
      Recording(const rdf::URI &uri, const std::string &filename, bool create=false) ;
      Recording(const std::string &filename, bool readonly=false) ;

      void close(void) override ;

      Clock::Ptr get_clock(const rdf::URI &uri) ;
      Clock::Ptr get_clock(const std::string &uri) ;
      std::list<rdf::URI> get_clock_uris(void) ;

      Signal::Ptr get_signal(const rdf::URI &uri) ;
      Signal::Ptr get_signal(const std::string &uri) ;
      std::list<rdf::URI> get_signal_uris(void) ;

      Clock::Ptr new_clock(const std::string &uri, const rdf::URI &units,
                                 double *times = nullptr, size_t datasize=0) ;

      Signal::Ptr new_signal(const std::string &uri, const rdf::URI &units, double rate) ;
      Signal::Ptr new_signal(const std::string &uri, const rdf::URI &units, Clock::Ptr clock) ;

      SignalArray::Ptr new_signalarray(const std::vector<std::string> &uris,
                                       const std::vector<rdf::URI> &units, double rate) ;
      SignalArray::Ptr new_signalarray(const std::vector<std::string> &uris,
                                       const std::vector<rdf::URI> &units, Clock::Ptr clock) ;
// Variants of new_signal() with rate/period (== regular Clock)

     private:
      File *m_file ;
      bool m_readonly ;
      std::set<std::shared_ptr<Dataset>> datasets ;
      } ;

    } ;

  } ;

#endif
