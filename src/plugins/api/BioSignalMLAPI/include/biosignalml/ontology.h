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

/**
Provide access to the BioSignalML ontology.

Generated from file:///Users/dave/biosignalml/workspace/ontologies/bsml/2011-04-biosignalml.ttl at 12:12:54 Wed 31 Dec 2014

Full documentation of the ontology is at http://www.biosignalml.org/ontologies/2011/04/biosignalml.html
**/

#ifndef BSML_ONTOLOGY_H
#define BSML_ONTOLOGY_H

#include <typedobject/rdfdefs.h>

namespace bsml {

  namespace BSML {
    NAMESPACE("bsml", "http://www.biosignalml.org/ontologies/2011/04/biosignalml#")
    TERM(Annotation)
    TERM(BP_Filter)
    TERM(Device)
    TERM(Electrode)
    TERM(ErrorTag)
    TERM(Event)
    TERM(EventType)
    TERM(Filter)
    TERM(HP_Filter)
    TERM(Instant)
    TERM(Interval)
    TERM(LP_Filter)
    TERM(Notch_Filter)
    TERM(Recording)
    TERM(RecordingGraph)
    TERM(SampleClock)
    TERM(Segment)
    TERM(SemanticTag)
    TERM(Sensor)
    TERM(Signal)
    TERM(SignalType)
    TERM(Simulation)
    TERM(Source)
    TERM(TemporalEntity)
    TERM(Transducer)
    TERM(UniformSignal)
    TERM(UnitOfMeasure)
    TERM(dataBits)
    TERM(filterFrequency)
    TERM(index)
    TERM(maxFrequency)
    TERM(maxValue)
    TERM(minFrequency)
    TERM(minValue)
    TERM(offset)
    TERM(period)
    TERM(rate)
    TERM(resolution)
    TERM(BP)
    TERM(ECG)
    TERM(EEG)
    TERM(ErrorTAG)
    TERM(clock)
    TERM(dataset)
    TERM(eventType)
    TERM(preFilter)
    TERM(recording)
    TERM(sensor)
    TERM(signalType)
    TERM(tag)
    TERM(time)
    TERM(uncertainty)
    TERM(units)
    } ;

  } ;

#endif
