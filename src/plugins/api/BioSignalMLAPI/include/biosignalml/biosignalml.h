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

#ifndef BIOSIGNALML_H
#define BIOSIGNALML_H

#include <string>

namespace bsml
/*==========*/
{
  const std::string VERSION = "0.8.1" ;
  } ;

#include <biosignalml/ontology.h>
#include <biosignalml/formats.h>
#include <biosignalml/resource.h>
#include <biosignalml/timing.h>
#include <biosignalml/signal.h>
#include <biosignalml/event.h>
#include <biosignalml/recording.h>
#include <biosignalml/segment.h>
#include <biosignalml/annotation.h>

#endif
