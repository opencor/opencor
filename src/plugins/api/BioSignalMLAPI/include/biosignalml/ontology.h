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

#include <typedobject/rdf.h>
#include <typedobject/rdfdefs.h>

namespace bsml {

  namespace BSML {
    static const rdf::Namespace NS("bsml", "http://www.biosignalml.org/ontologies/2011/04/biosignalml#") ;

    static const rdf::URI Annotation = NS.make_URI("Annotation") ;
    static const rdf::URI BP_Filter = NS.make_URI("BP_Filter") ;
    static const rdf::URI Device = NS.make_URI("Device") ;
    static const rdf::URI Electrode = NS.make_URI("Electrode") ;
    static const rdf::URI ErrorTag = NS.make_URI("ErrorTag") ;
    static const rdf::URI Event = NS.make_URI("Event") ;
    static const rdf::URI EventType = NS.make_URI("EventType") ;
    static const rdf::URI Filter = NS.make_URI("Filter") ;
    static const rdf::URI HP_Filter = NS.make_URI("HP_Filter") ;
    static const rdf::URI Instant = NS.make_URI("Instant") ;
    static const rdf::URI Interval = NS.make_URI("Interval") ;
    static const rdf::URI LP_Filter = NS.make_URI("LP_Filter") ;
    static const rdf::URI Notch_Filter = NS.make_URI("Notch_Filter") ;
    static const rdf::URI Recording = NS.make_URI("Recording") ;
    static const rdf::URI RecordingGraph = NS.make_URI("RecordingGraph") ;
    static const rdf::URI SampleClock = NS.make_URI("SampleClock") ;
    static const rdf::URI Segment = NS.make_URI("Segment") ;
    static const rdf::URI SemanticTag = NS.make_URI("SemanticTag") ;
    static const rdf::URI Sensor = NS.make_URI("Sensor") ;
    static const rdf::URI Signal = NS.make_URI("Signal") ;
    static const rdf::URI SignalType = NS.make_URI("SignalType") ;
    static const rdf::URI Simulation = NS.make_URI("Simulation") ;
    static const rdf::URI Source = NS.make_URI("Source") ;
    static const rdf::URI TemporalEntity = NS.make_URI("TemporalEntity") ;
    static const rdf::URI Transducer = NS.make_URI("Transducer") ;
    static const rdf::URI UniformSignal = NS.make_URI("UniformSignal") ;
    static const rdf::URI UnitOfMeasure = NS.make_URI("UnitOfMeasure") ;
    static const rdf::URI dataBits = NS.make_URI("dataBits") ;
    static const rdf::URI filterFrequency = NS.make_URI("filterFrequency") ;
    static const rdf::URI index = NS.make_URI("index") ;
    static const rdf::URI maxFrequency = NS.make_URI("maxFrequency") ;
    static const rdf::URI maxValue = NS.make_URI("maxValue") ;
    static const rdf::URI minFrequency = NS.make_URI("minFrequency") ;
    static const rdf::URI minValue = NS.make_URI("minValue") ;
    static const rdf::URI offset = NS.make_URI("offset") ;
    static const rdf::URI period = NS.make_URI("period") ;
    static const rdf::URI rate = NS.make_URI("rate") ;
    static const rdf::URI resolution = NS.make_URI("resolution") ;
    static const rdf::URI BP = NS.make_URI("BP") ;
    static const rdf::URI ECG = NS.make_URI("ECG") ;
    static const rdf::URI EEG = NS.make_URI("EEG") ;
    static const rdf::URI ErrorTAG = NS.make_URI("ErrorTAG") ;
    static const rdf::URI clock = NS.make_URI("clock") ;
    static const rdf::URI dataset = NS.make_URI("dataset") ;
    static const rdf::URI eventType = NS.make_URI("eventType") ;
    static const rdf::URI preFilter = NS.make_URI("preFilter") ;
    static const rdf::URI recording = NS.make_URI("recording") ;
    static const rdf::URI sensor = NS.make_URI("sensor") ;
    static const rdf::URI signalType = NS.make_URI("signalType") ;
    static const rdf::URI tag = NS.make_URI("tag") ;
    static const rdf::URI time = NS.make_URI("time") ;
    static const rdf::URI uncertainty = NS.make_URI("uncertainty") ;
    static const rdf::URI units = NS.make_URI("units") ;
    } ;

  } ;

#endif
