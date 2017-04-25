/**
 * @file fieldmodule.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDMODULE_HPP__
#define CMZN_FIELDMODULE_HPP__

#include "opencmiss/zinc/fieldmodule.h"
#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/element.hpp"
#include "opencmiss/zinc/node.hpp"
#include "opencmiss/zinc/region.hpp"
#include "opencmiss/zinc/timesequence.hpp"
#include "opencmiss/zinc/types/scenecoordinatesystem.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldAlias;
class FieldAdd;
class FieldPower;
class FieldMultiply;
class FieldDivide;
class FieldSubtract;
class FieldSumComponents;
class FieldLog;
class FieldSqrt;
class FieldExp;
class FieldAbs;
class FieldIdentity;
class FieldComponent;
class FieldConcatenate;
class FieldIf;
class FieldConstant;
class FieldStringConstant;
class FieldCoordinateTransformation;
class FieldVectorCoordinateTransformation;
class FieldFibreAxes;
class FieldFiniteElement;
class FieldEdgeDiscontinuity;
class FieldEmbedded;
class FieldFindMeshLocation;
class FieldNodeValue;
class FieldStoredMeshLocation;
class FieldStoredString;
class FieldIsExterior;
class FieldIsOnFace;
class FieldGroup;
class FieldImage;
class FieldAnd;
class FieldEqualTo;
class FieldGreaterThan;
class FieldIsDefined;
class FieldLessThan;
class FieldOr;
class FieldNot;
class FieldXor;
class FieldDeterminant;
class FieldEigenvalues;
class FieldEigenvectors;
class FieldMatrixInvert;
class FieldMatrixMultiply;
class FieldProjection;
class FieldTranspose;
class FieldMeshIntegral;
class FieldMeshIntegralSquares;
class FieldNodesetSum;
class FieldNodesetMean;
class FieldNodesetSumSquares;
class FieldNodesetMeanSquares;
class FieldNodesetMinimum;
class FieldNodesetMaximum;
class FieldElementGroup;
class FieldNodeGroup;
class FieldTimeLookup;
class FieldTimeValue;
class FieldDerivative;
class FieldCurl;
class FieldDivergence;
class FieldGradient;
class FieldSin;
class FieldCos;
class FieldTan;
class FieldAsin;
class FieldAcos;
class FieldAtan;
class FieldAtan2;
class FieldCrossProduct;
class FieldCrossProduct3D;
class FieldDotProduct;
class FieldMagnitude;
class FieldNodeLookup;
class FieldNormalise;
class FieldImagefilterBinaryDilate;
class FieldImagefilterBinaryErode;
class FieldImagefilterBinaryThreshold;
class FieldImagefilterCannyEdgeDetection;
class FieldImagefilterConnectedThreshold;
class FieldImagefilterCurvatureAnisotropicDiffusion;
class FieldImagefilterDiscreteGaussian;
class FieldImagefilterHistogram;
class FieldImagefilterMean;
class FieldImagefilterGradientMagnitudeRecursiveGaussian;
class FieldImagefilterRescaleIntensity;
class FieldImagefilterSigmoid;
class FieldImagefilterThreshold;
class FieldSceneviewerProjection;
class Fieldcache;
class Fieldmodulenotifier;
class Fieldsmoothing;
class Timekeeper;
class Optimisation;
class Sceneviewer;

class Fieldmodule
{
private:

	cmzn_fieldmodule_id id;

public:

	Fieldmodule() : id(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit Fieldmodule(cmzn_fieldmodule_id field_module_id) :
		id(field_module_id)
	{ }

	Fieldmodule(const Fieldmodule& fieldModule) :
		id(cmzn_fieldmodule_access(fieldModule.id))
	{ }

	Fieldmodule& operator=(const Fieldmodule& fieldModule)
	{
		cmzn_fieldmodule_id temp_id = cmzn_fieldmodule_access(fieldModule.id);
		if (0 != id)
		{
			cmzn_fieldmodule_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Fieldmodule()
	{
		if (0 != id)
		{
			cmzn_fieldmodule_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fieldmodule_id getId() const
	{
		return id;
	}

	int beginChange()
	{
		return cmzn_fieldmodule_begin_change(id);
	}

	int endChange()
	{
		return cmzn_fieldmodule_end_change(id);
	}

	int defineAllFaces()
	{
		return cmzn_fieldmodule_define_all_faces(id);
	}

	Field findFieldByName(const char *fieldName)
	{
		return Field(cmzn_fieldmodule_find_field_by_name(id, fieldName));
	}

	inline Fieldcache createFieldcache();

	Fielditerator createFielditerator()
	{
		return Fielditerator(cmzn_fieldmodule_create_fielditerator(id));
	}

	Fieldmodulenotifier createFieldmodulenotifier();

	inline Fieldsmoothing createFieldsmoothing();

	Elementbasis createElementbasis(int dimension, enum Elementbasis::FunctionType functionType)
	{
		return Elementbasis(cmzn_fieldmodule_create_elementbasis(
			id, dimension, static_cast<cmzn_elementbasis_function_type>(functionType)));
	}

	Nodeset findNodesetByFieldDomainType(Field::DomainType domainType)
	{
		return Nodeset(cmzn_fieldmodule_find_nodeset_by_field_domain_type(id,
			static_cast<cmzn_field_domain_type>(domainType)));
	}

	Nodeset findNodesetByName(const char *nodeset_name)
	{
		return Nodeset(cmzn_fieldmodule_find_nodeset_by_name(id,
			nodeset_name));
	}

	Mesh findMeshByDimension(int dimension)
	{
		return Mesh(cmzn_fieldmodule_find_mesh_by_dimension(id, dimension));
	}

	Mesh findMeshByName(const char *meshName)
	{
		return Mesh(cmzn_fieldmodule_find_mesh_by_name(id, meshName));
	}

	char *writeDescription()
	{
		return cmzn_fieldmodule_write_description(id);
	}

	int readDescription(const char *description)
	{
		return cmzn_fieldmodule_read_description(id, description);
	}

	inline Timesequence getMatchingTimesequence(int timesCount, const double *timesIn)
	{
		return Timesequence(cmzn_fieldmodule_get_matching_timesequence(
			id, timesCount, timesIn));
	}

	inline Region getRegion() const
	{
		return Region(cmzn_fieldmodule_get_region(id));
	}

	inline Optimisation createOptimisation();

	inline FieldAlias createFieldAlias(const Field& sourceField);

	inline FieldAdd createFieldAdd(const Field& sourceField1, const Field& sourceField2);

	inline FieldPower createFieldPower(const Field& sourceField1, const Field& sourceField2);

	inline FieldMultiply createFieldMultiply(const Field& sourceField1, const Field& sourceField2);

	inline FieldDivide createFieldDivide(const Field& sourceField1, const Field& sourceField2);

	inline FieldSubtract createFieldSubtract(const Field& sourceField1, const Field& sourceField2);

	inline FieldSumComponents createFieldSumComponents(const Field& sourceField);

	inline FieldLog createFieldLog(const Field& sourceField);

	inline FieldSqrt createFieldSqrt(const Field& sourceField);

	inline FieldExp createFieldExp(const Field& sourceField);

	inline FieldAbs createFieldAbs(const Field& sourceField);

	inline FieldIdentity createFieldIdentity(const Field& sourceField);

	inline FieldComponent createFieldComponent(const Field& sourceField, int sourceComponentIndex);

	inline FieldComponent createFieldComponent(const Field& sourceField,
		int sourceComponentIndexesCount, const int *sourceComponentIndexesIn);

	inline FieldConcatenate createFieldConcatenate(int fieldsCount, const Field *sourceFields);

	inline FieldIf createFieldIf(const Field& sourceField1, const Field& sourceField2, const Field& sourceField3);

	inline FieldConstant createFieldConstant(int valuesCount, const double *valuesIn);

	inline FieldStringConstant createFieldStringConstant(const char *stringConstant);

	inline FieldCoordinateTransformation createFieldCoordinateTransformation(const Field& sourceField);

	inline FieldVectorCoordinateTransformation createFieldVectorCoordinateTransformation(
		const Field& vectorField, const Field& coordinateField);

	inline FieldFibreAxes createFieldFibreAxes(const Field& fibreField, const Field& coordinateField);

	inline FieldFiniteElement createFieldFiniteElement(int numberOfComponents);

	inline FieldEmbedded createFieldEmbedded(const Field& sourceField, const Field& embeddedLocationField);

	inline FieldEdgeDiscontinuity createFieldEdgeDiscontinuity(const Field& sourceField);

	inline FieldFindMeshLocation createFieldFindMeshLocation(
		const Field& sourceField, const Field& meshField, const Mesh& mesh);

	inline FieldNodeValue createFieldNodeValue(const Field& sourceField,
		Node::ValueLabel nodeValueLabel, int versionNumber);

	inline FieldStoredMeshLocation createFieldStoredMeshLocation(const Mesh& mesh);

	inline FieldStoredString createFieldStoredString();

	inline FieldIsExterior createFieldIsExterior();

	inline FieldIsOnFace createFieldIsOnFace(Element::FaceType face);

	inline FieldGroup createFieldGroup();

	inline FieldImage createFieldImage();

	inline FieldImage createFieldImageFromSource(const Field& sourceField);

	inline FieldAnd createFieldAnd(const Field& sourceField1, const Field& sourceField2);

	inline FieldEqualTo createFieldEqualTo(const Field& sourceField1, const Field& sourceField2);

	inline FieldGreaterThan createFieldGreaterThan(const Field& sourceField1, const Field& sourceField2);

	inline FieldIsDefined createFieldIsDefined(const Field& sourceField);

	inline FieldLessThan createFieldLessThan(const Field& sourceField1, const Field& sourceField2);

	inline FieldOr createFieldOr(const Field& sourceField1, const Field& sourceField2);

	inline FieldNot createFieldNot(const Field& sourceField);

	inline FieldXor createFieldXor(const Field& sourceField1, const Field& sourceField2);

	inline FieldDeterminant createFieldDeterminant(const Field& sourceField);

	inline FieldEigenvalues createFieldEigenvalues(const Field& sourceField);

	inline FieldEigenvectors createFieldEigenvectors(const FieldEigenvalues& eigenValuesField);

	inline FieldMatrixInvert createFieldMatrixInvert(const Field& sourceField);

	inline FieldMatrixMultiply createFieldMatrixMultiply(int numberOfRows,
		const Field& sourceField1, const Field& sourceField2);

	inline FieldProjection createFieldProjection(const Field& sourceField,	const Field& projectionMatrixField);

	inline FieldTranspose createFieldTranspose(int sourceNumberOfRows, const Field& sourceField);

	inline FieldMeshIntegral createFieldMeshIntegral(const Field& integrandField,
		const Field& coordinateField, const Mesh& mesh);

	inline FieldMeshIntegralSquares createFieldMeshIntegralSquares(const Field& integrandField,
		const Field& coordinateField, const Mesh& mesh);

	inline FieldNodesetSum createFieldNodesetSum(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodesetMean createFieldNodesetMean(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodesetSumSquares createFieldNodesetSumSquares(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodesetMeanSquares createFieldNodesetMeanSquares(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodesetMinimum createFieldNodesetMinimum(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodesetMaximum createFieldNodesetMaximum(const Field& sourceField, const Nodeset& nodeset);

	inline FieldNodeGroup createFieldNodeGroup(const Nodeset& nodeset);

	inline FieldElementGroup createFieldElementGroup(const Mesh& mesh);

	inline FieldTimeLookup createFieldTimeLookup(const Field& sourceField, const Field& timeField);

	inline FieldTimeValue createFieldTimeValue(const Timekeeper& timeKeeper);

	inline FieldDerivative createFieldDerivative(const Field& sourceField, int xi_index);

	inline FieldCurl createFieldCurl(const Field& vectorField, const Field& coordinateField);

	inline FieldDivergence createFieldDivergence(const Field& vectorField, const Field& coordinateField);

	inline FieldGradient createFieldGradient(const Field& sourceField, const Field& coordinateField);

	inline FieldSin createFieldSin(const Field& sourceField);

	inline FieldCos createFieldCos(const Field& sourceField);

	inline FieldTan createFieldTan(const Field& sourceField);

	inline FieldAsin createFieldAsin(const Field& sourceField);

	inline FieldAcos createFieldAcos(const Field& sourceField);

	inline FieldAtan createFieldAtan(const Field& sourceField);

	inline FieldAtan2 createFieldAtan2(const Field& sourceField1, const Field& sourceField2);

	inline FieldCrossProduct createFieldCrossProduct(int fieldsCount, const Field *sourceFields);

	inline FieldCrossProduct createFieldCrossProduct(const Field& sourceField1, const Field& sourceField2);

	inline FieldDotProduct createFieldDotProduct(const Field& sourceField1, const Field& sourceField2);

	inline FieldMagnitude createFieldMagnitude(const Field& sourceField);

	inline FieldNodeLookup createFieldNodeLookup(const Field& sourceField, const Node& lookupNode);

	inline FieldNormalise createFieldNormalise(const Field& sourceField);

	inline FieldImagefilterBinaryDilate createFieldImagefilterBinaryDilate(const Field& sourceField,
		int radius, double dilate_value);

	inline FieldImagefilterBinaryErode createFieldImagefilterBinaryErode(const Field& sourceField,
		int radius, double erode_value);

	inline FieldImagefilterBinaryThreshold createFieldImagefilterBinaryThreshold(const Field& sourceField);

	inline FieldImagefilterCannyEdgeDetection createFieldImagefilterCannyEdgeDetection(const Field& sourceField,
			double variance, double maximumError, double upperThreshold, double lowerThreshold);

	inline FieldImagefilterConnectedThreshold createFieldImagefilterConnectedThreshold(const Field& sourceField,
		double lowerThreshold, double upperThreshold, double replaceValue,
		int dimension, int seedPointsCount, const double *seedPoints);

	inline FieldImagefilterCurvatureAnisotropicDiffusion createFieldImagefilterCurvatureAnisotropicDiffusion(
		const Field& sourceField, double timeStep, double conductance, int numIterations);

	inline FieldImagefilterDiscreteGaussian createFieldImagefilterDiscreteGaussian(const Field& sourceField);

	inline FieldImagefilterHistogram createFieldImagefilterHistogram(const Field& sourceField);

	inline FieldImagefilterMean createFieldImagefilterMean(const Field& sourceField, int radiusSizesCount,
		const int *radiusSizesIn);

	inline FieldImagefilterGradientMagnitudeRecursiveGaussian
		createFieldImagefilterGradientMagnitudeRecursiveGaussian(const Field& sourceField,
			double sigma);

	inline FieldImagefilterRescaleIntensity createFieldImagefilterRescaleIntensity(const Field& sourceField,
		double outputMin, double outputMax);

	inline FieldImagefilterSigmoid createFieldImagefilterSigmoid(const Field& sourceField,
		double min, double max,	double alpha, double beta);

	inline FieldImagefilterThreshold createFieldImagefilterThreshold(const Field& sourceField);

	inline FieldSceneviewerProjection createFieldSceneviewerProjection(
		const Sceneviewer& sceneviewer, Scenecoordinatesystem fromCoordinateSystem,
		Scenecoordinatesystem toCoordinateSystem);
};

inline bool operator==(const Fieldmodule& a, const Fieldmodule& b)
{
	return cmzn_fieldmodule_match(a.getId(), b.getId());
}

class Fieldmoduleevent
{
protected:
	cmzn_fieldmoduleevent_id id;

public:

	Fieldmoduleevent() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fieldmoduleevent(cmzn_fieldmoduleevent_id in_fieldmodule_event_id) :
		id(in_fieldmodule_event_id)
	{  }

	Fieldmoduleevent(const Fieldmoduleevent& fieldmoduleEvent) :
		id(cmzn_fieldmoduleevent_access(fieldmoduleEvent.id))
	{  }

	Fieldmoduleevent& operator=(const Fieldmoduleevent& fieldmoduleEvent)
	{
		cmzn_fieldmoduleevent_id temp_id = cmzn_fieldmoduleevent_access(fieldmoduleEvent.id);
		if (0 != id)
			cmzn_fieldmoduleevent_destroy(&id);
		id = temp_id;
		return *this;
	}

	~Fieldmoduleevent()
	{
		if (0 != id)
		{
			cmzn_fieldmoduleevent_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fieldmoduleevent_id getId() const
	{
		return id;
	}

	Field::ChangeFlags getFieldChangeFlags(const Field& field) const
	{
		return cmzn_fieldmoduleevent_get_field_change_flags(id, field.getId());
	}

	Meshchanges getMeshchanges(const Mesh& mesh) const
	{
		return Meshchanges(cmzn_fieldmoduleevent_get_meshchanges(id, mesh.getId()));
	}

	Nodesetchanges getNodesetchanges(const Nodeset& nodeset) const
	{
		return Nodesetchanges(cmzn_fieldmoduleevent_get_nodesetchanges(id, nodeset.getId()));
	}

	Field::ChangeFlags getSummaryFieldChangeFlags() const
	{
		return cmzn_fieldmoduleevent_get_summary_field_change_flags(id);
	}

};

/**
 * @brief Base class functor for field module notifier callbacks
 *
 * Base class functor for field module notifier callbacks:
 * - Derive from this class adding any user data required.
 * - Implement virtual operator()(const Fieldmoduleevent&) to handle callback.
 * @see Fieldmodulenotifier::setCallback()
 */
class Fieldmodulecallback
{
friend class Fieldmodulenotifier;
private:
	Fieldmodulecallback(const Fieldmodulecallback&); // not implemented
	Fieldmodulecallback& operator=(const Fieldmodulecallback&); // not implemented

	static void C_callback(cmzn_fieldmoduleevent_id fieldmoduleevent_id, void *callbackVoid)
	{
		Fieldmoduleevent fieldmoduleevent(cmzn_fieldmoduleevent_access(fieldmoduleevent_id));
		Fieldmodulecallback *callback = reinterpret_cast<Fieldmodulecallback *>(callbackVoid);
		(*callback)(fieldmoduleevent);
	}

  virtual void operator()(const Fieldmoduleevent &fieldmoduleevent) = 0;

protected:
	Fieldmodulecallback()
	{ }

public:
	virtual ~Fieldmodulecallback()
	{ }
};

class Fieldmodulenotifier
{
protected:
	cmzn_fieldmodulenotifier_id id;

public:

	Fieldmodulenotifier() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Fieldmodulenotifier(cmzn_fieldmodulenotifier_id in_fieldmodulenotifier_id) :
		id(in_fieldmodulenotifier_id)
	{  }

	Fieldmodulenotifier(const Fieldmodulenotifier& fieldmoduleNotifier) :
		id(cmzn_fieldmodulenotifier_access(fieldmoduleNotifier.id))
	{  }

	Fieldmodulenotifier& operator=(const Fieldmodulenotifier& fieldmoduleNotifier)
	{
		cmzn_fieldmodulenotifier_id temp_id = cmzn_fieldmodulenotifier_access(fieldmoduleNotifier.id);
		if (0 != id)
		{
			cmzn_fieldmodulenotifier_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Fieldmodulenotifier()
	{
		if (0 != id)
		{
			cmzn_fieldmodulenotifier_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_fieldmodulenotifier_id getId() const
	{
		return id;
	}

	int setCallback(Fieldmodulecallback& callback)
	{
		return cmzn_fieldmodulenotifier_set_callback(id, callback.C_callback, static_cast<void*>(&callback));
	}

	int clearCallback()
	{
		return cmzn_fieldmodulenotifier_clear_callback(id);
	}
};

inline Fieldmodule Region::getFieldmodule() const
{
	return Fieldmodule(cmzn_region_get_fieldmodule(id));
}

inline Fieldmodule Field::getFieldmodule() const
{
	return Fieldmodule(cmzn_field_get_fieldmodule(id));
}

inline Fieldmodule Mesh::getFieldmodule() const
{
	return Fieldmodule(cmzn_mesh_get_fieldmodule(id));
}

inline Fieldmodule Nodeset::getFieldmodule() const
{
	return Fieldmodule(cmzn_nodeset_get_fieldmodule(id));
}

inline Fieldmodulenotifier Fieldmodule::createFieldmodulenotifier()
{
	return Fieldmodulenotifier(cmzn_fieldmodule_create_fieldmodulenotifier(id));
}

}  // namespace Zinc
}

#endif
