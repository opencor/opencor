/**
 * @file    SEDMLTypes.h
 * @brief   Include all SEDML types in a single header file.
 *
 * <!--------------------------------------------------------------------------
 *
 * This file is part of libSEDML.  Please visit http://sed-ml.org for more
 * information about SED-ML. The latest version of libSEDML can be found on
 * github: https://github.com/fbergmann/libSEDML/
 *
 *
 * Copyright (c) 2013-2014, Frank T. Bergmann
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---------------------------------------------------------------------- -->
 */

#ifndef SEDMLTypes_h
#define SEDMLTypes_h


#include <sedml/common/sedmlfwd.h>

#include <sedml/SedVariable.h>
#include <sedml/SedParameter.h>
#include <sedml/SedChange.h>
#include <sedml/SedChangeAttribute.h>
#include <sedml/SedAddXML.h>
#include <sedml/SedChangeXML.h>
#include <sedml/SedRemoveXML.h>
#include <sedml/SedComputeChange.h>
#include <sedml/SedModel.h>
#include <sedml/SedAlgorithm.h>
#include <sedml/SedAlgorithmParameter.h>
#include <sedml/SedSimulation.h>
#include <sedml/SedUniformTimeCourse.h>
#include <sedml/SedOneStep.h>
#include <sedml/SedSteadyState.h>
#include <sedml/SedDataSet.h>
#include <sedml/SedCurve.h>
#include <sedml/SedSurface.h>
#include <sedml/SedOutput.h>
#include <sedml/SedReport.h>
#include <sedml/SedPlot2D.h>
#include <sedml/SedPlot3D.h>
#include <sedml/SedDataGenerator.h>
#include <sedml/SedTask.h>
#include <sedml/SedRepeatedTask.h>
#include <sedml/SedSubTask.h>
#include <sedml/SedSetValue.h>
#include <sedml/SedRange.h>
#include <sedml/SedUniformRange.h>
#include <sedml/SedVectorRange.h>
#include <sedml/SedFunctionalRange.h>
#include <sedml/SedDocument.h>

#include <sedml/SedBase.h>
#include <sedml/SedListOf.h>


#include <sedml/SedReader.h>
#include <sedml/SedWriter.h>

#include <sbml/xml/XMLError.h>
#include <sbml/math/ASTNode.h>
#include <sbml/math/MathML.h>

#endif  /* SEDMLTypes_h */
