/**
 * @file fieldimageprocessing.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_FIELDIMAGEPROCESSING_HPP__
#define CMZN_FIELDIMAGEPROCESSING_HPP__

#include "opencmiss/zinc/field.hpp"
#include "opencmiss/zinc/fieldimageprocessing.h"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class FieldImagefilterBinaryDilate : public Field
{

private:
	explicit FieldImagefilterBinaryDilate(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterBinaryDilate
		Fieldmodule::createFieldImagefilterBinaryDilate(const Field& sourceField,
			int radius, double dilate_value);

public:

	FieldImagefilterBinaryDilate() : Field(0)
	{	}

};

class FieldImagefilterBinaryErode : public Field
{

private:
	explicit FieldImagefilterBinaryErode(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterBinaryErode
		Fieldmodule::createFieldImagefilterBinaryErode(const Field& sourceField,
			int radius, double erode_value);

public:

	FieldImagefilterBinaryErode() : Field(0)
	{	}

};

class FieldImagefilterBinaryThreshold : public Field
{
public:

	FieldImagefilterBinaryThreshold() : Field(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterBinaryThreshold(cmzn_field_imagefilter_binary_threshold_id field_imagefilter_binary_threshold_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_imagefilter_binary_threshold_id))
	{	}

	double getLowerThreshold()
	{
		return cmzn_field_imagefilter_binary_threshold_get_lower_threshold(
			reinterpret_cast<cmzn_field_imagefilter_binary_threshold_id>(id));
	}

	int setLowerThreshold(double lowerThreshold)
	{
		return cmzn_field_imagefilter_binary_threshold_set_lower_threshold(
			reinterpret_cast<cmzn_field_imagefilter_binary_threshold_id>(id),
			lowerThreshold);
	}

	double getUpperThreshold()
	{
		return cmzn_field_imagefilter_binary_threshold_get_upper_threshold(
			reinterpret_cast<cmzn_field_imagefilter_binary_threshold_id>(id));
	}

	int setUpperThreshold(double upperThreshold)
	{
		return cmzn_field_imagefilter_binary_threshold_set_upper_threshold(
			reinterpret_cast<cmzn_field_imagefilter_binary_threshold_id>(id),
			upperThreshold);
	}

};

class FieldImagefilterCannyEdgeDetection : public Field
{

private:
	explicit FieldImagefilterCannyEdgeDetection(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterCannyEdgeDetection
		Fieldmodule::createFieldImagefilterCannyEdgeDetection(const Field& sourceField,
			double variance, double maximumError, double upperThreshold, double lowerThreshold);

public:

	FieldImagefilterCannyEdgeDetection() : Field(0)
	{	}

};

class FieldImagefilterConnectedThreshold : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterConnectedThreshold(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterConnectedThreshold
		Fieldmodule::createFieldImagefilterConnectedThreshold(const Field& sourceField,
			  double lowerThreshold, double upperThreshold, double replaceValue,
			  int dimension, int seedPointsCount, const double *seedPoints);

public:

	FieldImagefilterConnectedThreshold() : Field(0)
	{	}

};

class FieldImagefilterCurvatureAnisotropicDiffusion : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterCurvatureAnisotropicDiffusion(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterCurvatureAnisotropicDiffusion
		Fieldmodule::createFieldImagefilterCurvatureAnisotropicDiffusion(const Field& sourceField,
			double timeStep, double conductance, int numIterations);

public:

	FieldImagefilterCurvatureAnisotropicDiffusion() : Field(0)
	{	}

};

class FieldImagefilterDiscreteGaussian : public Field
{
public:

	FieldImagefilterDiscreteGaussian() : Field(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterDiscreteGaussian(cmzn_field_imagefilter_discrete_gaussian_id field_imagefilter_discrete_gaussian_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_imagefilter_discrete_gaussian_id))
	{	}

	double getVariance()
	{
		return cmzn_field_imagefilter_discrete_gaussian_get_variance(
			reinterpret_cast<cmzn_field_imagefilter_discrete_gaussian_id>(id));
	}

	int setVariance(double variance)
	{
		return cmzn_field_imagefilter_discrete_gaussian_set_variance(
			reinterpret_cast<cmzn_field_imagefilter_discrete_gaussian_id>(id),
			variance);
	}

	int getMaxKernelWidth()
	{
		return cmzn_field_imagefilter_discrete_gaussian_get_max_kernel_width(
			reinterpret_cast<cmzn_field_imagefilter_discrete_gaussian_id>(id));
	}

	int setMaxKernelWidth(int maxKernelWidth)
	{
		return cmzn_field_imagefilter_discrete_gaussian_set_max_kernel_width(
			reinterpret_cast<cmzn_field_imagefilter_discrete_gaussian_id>(id),
			maxKernelWidth);
	}

};

class FieldImagefilterHistogram : public Field
{
public:

	FieldImagefilterHistogram() : Field(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterHistogram(cmzn_field_imagefilter_histogram_id field_imagefilter_histogram_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_imagefilter_histogram_id))
	{	}

	int getComputeMinimumValues(int valuesCount, double *valuesOut)
	{
		return cmzn_field_imagefilter_histogram_get_compute_minimum_values(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesOut);
	}

	int setComputeMinimumValues(int valuesCount, const double *valuesIn)
	{
		return cmzn_field_imagefilter_histogram_set_compute_minimum_values(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesIn);
	}

	int getComputeMaximumValues(int valuesCount, double *valuesOut)
	{
		return cmzn_field_imagefilter_histogram_get_compute_maximum_values(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesOut);
	}

	int setComputeMaximumValues(int valuesCount, const double *valuesIn)
	{
		return cmzn_field_imagefilter_histogram_set_compute_maximum_values(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesIn);
	}

	int getNumberOfBins(int valuesCount, int *valuesOut)
	{
		return cmzn_field_imagefilter_histogram_get_number_of_bins(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesOut);
	}

	int setNumberOfBins(int valuesCount, const int *valuesIn)
	{
		return cmzn_field_imagefilter_histogram_set_number_of_bins(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			valuesCount, valuesIn);
	}

	double getMarginalScale()
	{
		return cmzn_field_imagefilter_histogram_get_marginal_scale(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id));
	}

	int setMarginalScale(double marginalScale)
	{
		return cmzn_field_imagefilter_histogram_set_marginal_scale(
			reinterpret_cast<cmzn_field_imagefilter_histogram_id>(id),
			marginalScale);
	}

};

class FieldImagefilterGradientMagnitudeRecursiveGaussian : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterGradientMagnitudeRecursiveGaussian(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterGradientMagnitudeRecursiveGaussian
		Fieldmodule::createFieldImagefilterGradientMagnitudeRecursiveGaussian(const Field& sourceField,
			double sigma);

public:

	FieldImagefilterGradientMagnitudeRecursiveGaussian() : Field(0)
	{	}

};

class FieldImagefilterRescaleIntensity : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterRescaleIntensity(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterRescaleIntensity
		Fieldmodule::createFieldImagefilterRescaleIntensity(const Field& sourceField,
			double outputMin, double outputMax);

public:

	FieldImagefilterRescaleIntensity() : Field(0)
	{	}

};


class FieldImagefilterMean : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterMean(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterMean
		Fieldmodule::createFieldImagefilterMean(const Field& sourceField,
			int radiusSizesCount, const int *radiusSizesIn);

public:

	FieldImagefilterMean() : Field(0)
	{	}

};

class FieldImagefilterSigmoid : public Field
{

private:
	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterSigmoid(cmzn_field_id field_id) : Field(field_id)
	{	}

	friend FieldImagefilterSigmoid
		Fieldmodule::createFieldImagefilterSigmoid(const Field& sourceField,
			double min, double max,	double alpha, double beta);

public:

	FieldImagefilterSigmoid() : Field(0)
	{	}

};


class FieldImagefilterThreshold : public Field
{
public:

	FieldImagefilterThreshold() : Field(0)
	{	}

	// takes ownership of C handle, responsibility for destroying it
	explicit FieldImagefilterThreshold(cmzn_field_imagefilter_threshold_id field_imagefilter_threshold_id) :
		Field(reinterpret_cast<cmzn_field_id>(field_imagefilter_threshold_id))
	{	}

	enum Condition
	{
		CONDITION_INVALID = CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_INVALID,
		CONDITION_ABOVE = CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_ABOVE,
		CONDITION_BELOW = CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_BELOW,
		CONDITION_OUTSIDE = CMZN_FIELD_IMAGEFILTER_THRESHOLD_CONDITION_OUTSIDE
	};

	enum Condition getCondition()
	{
		return static_cast<Condition>(cmzn_field_imagefilter_threshold_get_condition(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id)));
	}

	int setCondition(Condition condition)
	{
		return cmzn_field_imagefilter_threshold_set_condition(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id),
			static_cast<cmzn_field_imagefilter_threshold_condition>(condition));
	}

	double getOutsideValue()
	{
		return cmzn_field_imagefilter_threshold_get_outside_value(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id));
	}

	int setOutsideValue(double outsideValue)
	{
		return cmzn_field_imagefilter_threshold_set_outside_value(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id), outsideValue);
	}

	double getLowerThreshold()
	{
		return cmzn_field_imagefilter_threshold_get_lower_threshold(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id));
	}

	int setLowerThreshold(double lowerValue)
	{
		return cmzn_field_imagefilter_threshold_set_lower_threshold(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id), lowerValue);
	}

	double getUpperThreshold()
	{
		return cmzn_field_imagefilter_threshold_get_upper_threshold(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id));
	}

	int setUpperThreshold(double upperValue)
	{
		return cmzn_field_imagefilter_threshold_set_upper_threshold(
			reinterpret_cast<cmzn_field_imagefilter_threshold_id>(id), upperValue);
	}

};

inline FieldImagefilterBinaryDilate
	Fieldmodule::createFieldImagefilterBinaryDilate(const Field& sourceField,
		int radius, double dilate_value)
{
	return FieldImagefilterBinaryDilate(
		cmzn_fieldmodule_create_field_imagefilter_binary_dilate(id, sourceField.getId(),
			radius, dilate_value));
}

inline FieldImagefilterBinaryErode
	Fieldmodule::createFieldImagefilterBinaryErode(const Field& sourceField,
		int radius, double erode_value)
{
	return FieldImagefilterBinaryErode(
		cmzn_fieldmodule_create_field_imagefilter_binary_erode(id, sourceField.getId(),
			radius, erode_value));
}

inline FieldImagefilterBinaryThreshold
	Fieldmodule::createFieldImagefilterBinaryThreshold(const Field& sourceField)
{
	return FieldImagefilterBinaryThreshold(reinterpret_cast<cmzn_field_imagefilter_binary_threshold_id>(
		cmzn_fieldmodule_create_field_imagefilter_binary_threshold(id, sourceField.getId())));
}

inline FieldImagefilterBinaryThreshold Field::castImagefilterBinaryThreshold()
{
	return FieldImagefilterBinaryThreshold(cmzn_field_cast_imagefilter_binary_threshold(id));
}

inline FieldImagefilterCannyEdgeDetection
	Fieldmodule::createFieldImagefilterCannyEdgeDetection(const Field& sourceField,
		double variance, double maximumError, double upperThreshold, double lowerThreshold)
{
	return FieldImagefilterCannyEdgeDetection(
		cmzn_fieldmodule_create_field_imagefilter_canny_edge_detection(
			id, sourceField.getId(),
			variance, maximumError, upperThreshold, lowerThreshold));
}

inline FieldImagefilterConnectedThreshold
	Fieldmodule::createFieldImagefilterConnectedThreshold(const Field& sourceField,
		  double lowerThreshold, double upperThreshold, double replaceValue,
		  int dimension, int seedPointsCount, const double *seedPoints)
{
	return FieldImagefilterConnectedThreshold(
		cmzn_fieldmodule_create_field_imagefilter_connected_threshold(id, sourceField.getId(),
		lowerThreshold, upperThreshold, replaceValue, seedPointsCount, dimension, seedPoints));
}

inline FieldImagefilterCurvatureAnisotropicDiffusion
	Fieldmodule::createFieldImagefilterCurvatureAnisotropicDiffusion(const Field& sourceField,
		double timeStep, double conductance, int numIterations)
{
	return FieldImagefilterCurvatureAnisotropicDiffusion(
		cmzn_fieldmodule_create_field_imagefilter_curvature_anisotropic_diffusion(id, sourceField.getId(),
			timeStep, conductance, numIterations));
}

inline FieldImagefilterDiscreteGaussian
	Fieldmodule::createFieldImagefilterDiscreteGaussian(const Field& sourceField)
{
	return FieldImagefilterDiscreteGaussian(reinterpret_cast<cmzn_field_imagefilter_discrete_gaussian_id>(
		cmzn_fieldmodule_create_field_imagefilter_discrete_gaussian(id, sourceField.getId())));
}

inline FieldImagefilterDiscreteGaussian Field::castImagefilterDiscreteGaussian()
{
	return FieldImagefilterDiscreteGaussian(cmzn_field_cast_imagefilter_discrete_gaussian(id));
}

inline FieldImagefilterGradientMagnitudeRecursiveGaussian
	Fieldmodule::createFieldImagefilterGradientMagnitudeRecursiveGaussian(const Field& sourceField,
		double sigma)
{
	return FieldImagefilterGradientMagnitudeRecursiveGaussian(
		cmzn_fieldmodule_create_field_imagefilter_gradient_magnitude_recursive_gaussian(id,
			sourceField.getId(), sigma));
}

inline FieldImagefilterHistogram
	Fieldmodule::createFieldImagefilterHistogram(const Field& sourceField)
{
	return FieldImagefilterHistogram(reinterpret_cast<cmzn_field_imagefilter_histogram_id>(
		cmzn_fieldmodule_create_field_imagefilter_histogram(id, sourceField.getId())));
}

inline FieldImagefilterHistogram Field::castImagefilterHistogram()
{
	return FieldImagefilterHistogram(cmzn_field_cast_imagefilter_histogram(id));
}

inline FieldImagefilterRescaleIntensity
	Fieldmodule::createFieldImagefilterRescaleIntensity(const Field& sourceField,
		double outputMin, double outputMax)
{
	return FieldImagefilterRescaleIntensity(
		cmzn_fieldmodule_create_field_imagefilter_rescale_intensity(id,
			sourceField.getId(), outputMin, outputMax));
}

inline FieldImagefilterMean Fieldmodule::createFieldImagefilterMean(const Field& sourceField,
	int radiusSizesCount, const int *radiusSizesIn)
{
	return FieldImagefilterMean(cmzn_fieldmodule_create_field_imagefilter_mean(
		id, sourceField.getId(), radiusSizesCount, radiusSizesIn));
}

inline FieldImagefilterSigmoid
	Fieldmodule::createFieldImagefilterSigmoid(const Field& sourceField,
		double min, double max,	double alpha, double beta)
{
	return FieldImagefilterSigmoid(
		cmzn_fieldmodule_create_field_imagefilter_sigmoid(id,
			sourceField.getId(), min, max, alpha, beta));
}

inline FieldImagefilterThreshold
	Fieldmodule::createFieldImagefilterThreshold(const Field& sourceField)
{
	return FieldImagefilterThreshold(reinterpret_cast<cmzn_field_imagefilter_threshold_id>(
		cmzn_fieldmodule_create_field_imagefilter_threshold(
			id, sourceField.getId())));
}

inline FieldImagefilterThreshold Field::castImagefilterThreshold()
{
	return FieldImagefilterThreshold(cmzn_field_cast_imagefilter_threshold(id));
}

}  // namespace Zinc
}

#endif
