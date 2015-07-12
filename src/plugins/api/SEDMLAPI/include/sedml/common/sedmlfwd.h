/**
 * @file    sedmlfwd.h
 * @brief   Forward declarations for all opaque C types.
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
 *
 * Declaring all types up-front avoids compilation errors of the form
 * <em>"Redefinition of type 'Foo'"</em>, and allows our combined C/C++
 * headers to depend minimally upon each other.  Put another way, the type
 * definitions below serve the same purpose as <tt>class Foo;</tt> forward
 * declarations in C++ code.
 */

#ifndef sedmlfwd_h
#define sedmlfwd_h


#include <sedml/common/libsedml-namespace.h>
#include <sedml/common/libsedml-config.h>


#ifdef __cplusplus
/**
 * Defined as a class when compiling for C++ and as a C struct when
 * compiling for C.
 */
#  define CLASS_OR_STRUCT class
#else
/**
 * Defined as a class when compiling for C++ and as a C struct when
 * compiling for C.
 */
#  define CLASS_OR_STRUCT struct
#endif  /* __cplusplus */

LIBSEDML_CPP_NAMESPACE_BEGIN

/**
 * @var typedef class SedBase SedBase_t
 * @copydoc SedBase
 */
typedef CLASS_OR_STRUCT SedBase                     SedBase_t;

/**
 * @var typedef class SedReader SedReader_t
 * @copydoc SedReader
 */
typedef CLASS_OR_STRUCT SedReader                     SedReader_t;

/**
 * @var typedef class SedWriter SedWriter_t
 * @copydoc SedWriter
 */
typedef CLASS_OR_STRUCT SedWriter                     SedWriter_t;


/**
 * @var typedef class SedNamespaces SedNamespaces_t
 * @copydoc SedNamespaces
 */
typedef CLASS_OR_STRUCT SedNamespaces              SedNamespaces_t;

/**
 * @var typedef class SedListOf SedListOf_t
 * @copydoc SedListOf
 */
typedef CLASS_OR_STRUCT SedListOf              SedListOf_t;


/**
 * @var typedef class SEDMLDocument SEDMLDocument_t
 * @copydoc SEDMLDocument
 */
typedef CLASS_OR_STRUCT SedDocument              SedDocument_t;


/**
 * @var typedef class SedModel SedModel_t
 * @copydoc SedModel
 */
typedef CLASS_OR_STRUCT SedModel                     SedModel_t;

/**
 * @var typedef class SedVariable SedVariable_t
 * @copydoc SedVariable
 */
typedef CLASS_OR_STRUCT SedVariable                     SedVariable_t;

/**
 * @var typedef class SedParameter SedParameter_t
 * @copydoc SedParameter
 */
typedef CLASS_OR_STRUCT SedParameter                     SedParameter_t;


/**
 * @var typedef class SedChange SedChange_t
 * @copydoc SedChange
 */
typedef CLASS_OR_STRUCT SedChange                     SedChange_t;

/**
 * @var typedef class SedRange SedRange_t
 * @copydoc SedRange
 */
typedef CLASS_OR_STRUCT SedRange                     SedRange_t;

/**
 * @var typedef class SedUniformRange SedUniformRange_t
 * @copydoc SedUniformRange
 */
typedef CLASS_OR_STRUCT SedUniformRange                     SedUniformRange_t;

/**
 * @var typedef class SedVectorRange SedVectorRange_t
 * @copydoc SedVectorRange
 */
typedef CLASS_OR_STRUCT SedVectorRange                     SedVectorRange_t;

/**
 * @var typedef class SedFunctionalRange SedFunctionalRange_t
 * @copydoc SedFunctionalRange
 */
typedef CLASS_OR_STRUCT SedFunctionalRange                     SedFunctionalRange_t;

/**
 * @var typedef class SedAlgorithm SedAlgorithm_t
 * @copydoc SedAlgorithm
 */
typedef CLASS_OR_STRUCT SedAlgorithm                     SedAlgorithm_t;

/**
 * @var typedef class SedOneStep SedOneStep_t
 * @copydoc SedOneStep
 */
typedef CLASS_OR_STRUCT SedOneStep                     SedOneStep_t;

/**
 * @var typedef class SedSteadyState SedSteadyState_t
 * @copydoc SedOneStep
 */
typedef CLASS_OR_STRUCT SedSteadyState                     SedSteadyState_t;

/**
 * @var typedef class SedSubTask SedSubTask_t
 * @copydoc SedOneStep
 */
typedef CLASS_OR_STRUCT SedSubTask                     SedSubTask_t;

/**
 * @var typedef class SedSetValue SedSetValue_t
 * @copydoc SedOneStep
 */
typedef CLASS_OR_STRUCT SedSetValue                     SedSetValue_t;


/**
 * @var typedef class SedAlgorithmParameter SedAlgorithmParameter_t
 * @copydoc SedAlgorithmParameter
 */
typedef CLASS_OR_STRUCT SedAlgorithmParameter                     SedAlgorithmParameter_t;


/**
 * @var typedef class SedComputeChange SedComputeChange_t
 * @copydoc SedComputeChange
 */
typedef CLASS_OR_STRUCT SedComputeChange                     SedComputeChange_t;


/**
 * @var typedef class SedRemoveXML SedRemoveXML_t
 * @copydoc SedRemoveXML
 */
typedef CLASS_OR_STRUCT SedRemoveXML                     SedRemoveXML_t;

/**
 * @var typedef class SedAddXML SedAddXML_t
 * @copydoc SedAddXML
 */
typedef CLASS_OR_STRUCT SedAddXML                     SedAddXML_t;


/**
 * @var typedef class SedChangeXML SedChangeXML_t
 * @copydoc SedChangeXML
 */
typedef CLASS_OR_STRUCT SedChangeXML                     SedChangeXML_t;


/**
 * @var typedef class SedChangeAttribute SedChangeAttribute_t
 * @copydoc SedChangeAttribute
 */
typedef CLASS_OR_STRUCT SedChangeAttribute                     SedChangeAttribute_t;

/**
 * @var typedef class SedDataGenerator SedDataGenerator_t
 * @copydoc SedDataGenerator
 */
typedef CLASS_OR_STRUCT SedDataGenerator                     SedDataGenerator_t;

/**
 * @var typedef class SedListOfDataGenerators SedListOfDataGenerators_t
 * @copydoc SedListOfDataGenerators
 */
typedef CLASS_OR_STRUCT SedListOfDataGenerators                     SedListOfDataGenerators_t;


/**
 * @var typedef class SedListOfModels SedListOfModels_t
 * @copydoc SedListOfModels
 */
typedef CLASS_OR_STRUCT SedListOfModels                     SedListOfModels_t;


/**
 * @var typedef class SedListOfOutputs SedListOfOutputs_t
 * @copydoc SedListOfOutputs
 */
typedef CLASS_OR_STRUCT SedListOfOutputs                     SedListOfOutputs_t;


/**
 * @var typedef class SedListOfTasks SedListOfTasks_t
 * @copydoc SedListOfTasks
 */
typedef CLASS_OR_STRUCT SedListOfTasks                     SedListOfTasks_t;


/**
 * @var typedef class SedTask SedTask_t
 * @copydoc Task
 */
typedef CLASS_OR_STRUCT SedTask                     SedTask_t;

/**
 * @var typedef class SedRepeatedTask SedRepeatedTask_t
 * @copydoc SedRepeatedTask
 */
typedef CLASS_OR_STRUCT SedRepeatedTask                     SedRepeatedTask_t;

/**
 * @var typedef class SedSimulation SedSimulation_t
 * @copydoc SedSimulation
 */
typedef CLASS_OR_STRUCT SedSimulation                     SedSimulation_t;


/**
 * @var typedef class SedUniformTimeCourse SedUniformTimeCourse_t
 * @copydoc SedUniformTimeCourse
 */
typedef CLASS_OR_STRUCT SedUniformTimeCourse                     SedUniformTimeCourse_t;


/**
 * @var typedef class SedOutput SedOutput_t
 * @copydoc Output
 */
typedef CLASS_OR_STRUCT SedOutput                     SedOutput_t;


/**
 * @var typedef class SedPlot3D SedPlot3D_t
 * @copydoc SedPlot3D
 */
typedef CLASS_OR_STRUCT SedPlot3D                     SedPlot3D_t;

/**
 * @var typedef class SedPlot2D SedPlot2D_t
 * @copydoc SedPlot2D
 */
typedef CLASS_OR_STRUCT SedPlot2D                     SedPlot2D_t;

/**
 * @var typedef class SedReport SedReport_t
 * @copydoc SedReport
 */
typedef CLASS_OR_STRUCT SedReport                     SedReport_t;

/**
 * @var typedef class SedSurface SedSurface_t
 * @copydoc SedSurface
 */
typedef CLASS_OR_STRUCT SedSurface                     SedSurface_t;

/**
 * @var typedef class SedCurve SedCurve_t
 * @copydoc SedCurve
 */
typedef CLASS_OR_STRUCT SedCurve                     SedCurve_t;

/**
 * @var typedef class SedDataSet SedDataSet_t
 * @copydoc SedDataSet
 */
typedef CLASS_OR_STRUCT SedDataSet                     SedDataSet_t;

LIBSEDML_CPP_NAMESPACE_END

LIBSBML_CPP_NAMESPACE_BEGIN

/**
 * @var typedef class ASTNode ASTNode_t
 * @copydoc ASTNode
 */
typedef CLASS_OR_STRUCT ASTNode                     ASTNode_t;

/**
 * @var typedef class XMLNode XMLNode_t
 * @copydoc XMLNode
 */
typedef CLASS_OR_STRUCT XMLNode                     XMLNode_t;

LIBSBML_CPP_NAMESPACE_END

#undef CLASS_OR_STRUCT

#endif  /* sedmlfwd_h  */

