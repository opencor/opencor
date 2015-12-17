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

#ifndef TYPEDOBJECT_UNITS_H
#define TYPEDOBJECT_UNITS_H

#include <typedobject/typedobject_export.h>

#include <string>

namespace Unit {

  class TYPEDOBJECT_EXPORT Converter
  /*------------------------------*/
  {
   public:
    Converter(const std::string & pFromUnits, const std::string & pToUnits);

    double convert(const double pValue) const;

   private:
    double mScale;
    double mOffset;
    };

  };

#endif

/*


Dimensions:
# reference
meter = [length] = m = metre
second = [time] = s = sec
ampere = [current] = A = amp
candela = [luminosity] = cd = candle
gram = [mass] = g
mole = [substance] = mol
kelvin = [temperature]; offset: 0 = K = degK
radian = [] = rad
bit = []
count = []


*/
