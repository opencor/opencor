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

#ifndef BSML_DATA_TIMESERIES_H
#define BSML_DATA_TIMESERIES_H

#include <biosignalml/biosignalml_export.h>
#include <typedobject/typedobject.h>

#include <vector>
#include <cmath>


namespace bsml {

  namespace data {

    class BIOSIGNALML_EXPORT Point
    /*--------------------------*/
    {
     public:
      Point() : m_time(NAN), m_data(NAN) { }
      Point(double t, double x) : m_time(t), m_data(x) { }
      virtual ~Point() = default ;

      double time(void) const { return m_time ; }
      double value(void) const { return m_data ; }

     private:
      double m_time ;
      double m_data ;
      } ;

    class BIOSIGNALML_EXPORT TimeSeries
    /*-------------------------------*/
    {
     public:
      SHARED_PTR(TimeSeries)
      TimeSeries() ;
      TimeSeries(const size_t size) ;
      TimeSeries(const std::vector<double> &times, const std::vector<double> &data) ;
      virtual ~TimeSeries() = default ;

      inline size_t size(void) const { return m_data.size() ; }
      inline const std::vector<double> & data(void) { return m_data ; }
      inline const std::vector<double> & times(void) { return m_times ; }
      virtual Point point(const size_t n) const ;

      // std::vector<Point> points(void) { return Point(m_times[n], m_data[n]) ; }
      // Can we use std::valarray<double> ??

     protected:
      std::vector<double> m_times ;
      std::vector<double> m_data ;
      } ;


    class BIOSIGNALML_EXPORT UniformTimeSeries : public TimeSeries
    /*----------------------------------------------------------*/
    {
     public:
      SHARED_PTR(UniformTimeSeries)
      UniformTimeSeries() ;
      UniformTimeSeries(const double rate, const size_t size) ;
      UniformTimeSeries(const double rate, const std::vector<double> &data) ;

      virtual Point point(const size_t n) const ;

     private:
      double m_rate ;
      } ;

    } ;

  } ;

#endif
