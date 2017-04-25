/**
 * @file optimisation.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_OPTIMISATION_HPP__
#define CMZN_OPTIMISATION_HPP__

#include "opencmiss/zinc/optimisation.h"
#include "opencmiss/zinc/fieldmodule.hpp"

namespace OpenCMISS
{
namespace Zinc
{

class Optimisation
{
protected:
	cmzn_optimisation_id id;

public:

	Optimisation() : id(0)
	{   }

	// takes ownership of C handle, responsibility for destroying it
	explicit Optimisation(cmzn_optimisation_id in_optimisation_id) :
		id(in_optimisation_id)
	{  }

	Optimisation(const Optimisation& optimisation) :
		id(cmzn_optimisation_access(optimisation.id))
	{ }

	Optimisation& operator=(const Optimisation& optimisation)
	{
		cmzn_optimisation_id temp_id = cmzn_optimisation_access(optimisation.id);
		if (0 != id)
		{
			cmzn_optimisation_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Optimisation()
	{
		if (0 != id)
		{
			cmzn_optimisation_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	enum Method
	{
		METHOD_INVALID = CMZN_OPTIMISATION_METHOD_INVALID,
		METHOD_QUASI_NEWTON = CMZN_OPTIMISATION_METHOD_QUASI_NEWTON,
		METHOD_LEAST_SQUARES_QUASI_NEWTON = CMZN_OPTIMISATION_METHOD_LEAST_SQUARES_QUASI_NEWTON
	};

	/**
	 * Labels of optimisation attributes which may be set or obtained using generic
	 * get/set_attribute functions.
	 */
	enum Attribute
	{
		ATTRIBUTE_FUNCTION_TOLERANCE = CMZN_OPTIMISATION_ATTRIBUTE_FUNCTION_TOLERANCE ,
		ATTRIBUTE_GRADIENT_TOLERANCE = CMZN_OPTIMISATION_ATTRIBUTE_GRADIENT_TOLERANCE,
		ATTRIBUTE_STEP_TOLERANCE = CMZN_OPTIMISATION_ATTRIBUTE_STEP_TOLERANCE,
		ATTRIBUTE_MAXIMUM_ITERATIONS = CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_ITERATIONS,
		ATTRIBUTE_MAXIMUM_FUNCTION_EVALUATIONS = CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_FUNCTION_EVALUATIONS,
		ATTRIBUTE_MAXIMUM_STEP = CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_STEP,
		ATTRIBUTE_MINIMUM_STEP = CMZN_OPTIMISATION_ATTRIBUTE_MINIMUM_STEP,
		ATTRIBUTE_LINESEARCH_TOLERANCE = CMZN_OPTIMISATION_ATTRIBUTE_LINESEARCH_TOLERANCE,
		ATTRIBUTE_MAXIMUM_BACKTRACK_ITERATIONS = CMZN_OPTIMISATION_ATTRIBUTE_MAXIMUM_BACKTRACK_ITERATIONS,
		ATTRIBUTE_TRUST_REGION_SIZE = CMZN_OPTIMISATION_ATTRIBUTE_TRUST_REGION_SIZE
	};

	cmzn_optimisation_id getId() const
	{
		return id;
	}

	Field getConditionalField(const Field& independentField)
	{
		return Field(cmzn_optimisation_get_conditional_field(id, independentField.getId()));
	}

	int setConditionalField(const Field& independentField, const Field& conditionalField)
	{
		return cmzn_optimisation_set_conditional_field(id, independentField.getId(), conditionalField.getId());
	}

	Method getMethod()
	{
		return static_cast<Method>(cmzn_optimisation_get_method(id));
	}

	int setMethod(Method method)
	{
		return cmzn_optimisation_set_method(id,
			static_cast<cmzn_optimisation_method>(method));
	}

	int getAttributeInteger(Attribute attribute)
	{
		return cmzn_optimisation_get_attribute_integer(id,
			static_cast<cmzn_optimisation_attribute>(attribute));
	}

	int setAttributeInteger(Attribute attribute, int value)
	{
		return cmzn_optimisation_set_attribute_integer(id,
			static_cast<cmzn_optimisation_attribute>(attribute), value);
	}

	double getAttributeReal(Attribute attribute)
	{
		return cmzn_optimisation_get_attribute_real(id,
					static_cast<cmzn_optimisation_attribute>(attribute));
	}

	int setAttributeReal(Attribute attribute, double value)
	{
		return cmzn_optimisation_set_attribute_real(id,
			static_cast<cmzn_optimisation_attribute>(attribute), value);
	}

	Field getFirstIndependentField()
	{
		return Field(cmzn_optimisation_get_first_independent_field(id));
	}

	Field getNextIndependentField(const Field& refField)
	{
		return Field(cmzn_optimisation_get_next_independent_field(id, refField.getId()));
	}

	int addIndependentField(const Field& field)
	{
		return (cmzn_optimisation_add_independent_field(id, field.getId()));
	}

	int removeIndependentField(const Field& field)
	{
		return (cmzn_optimisation_remove_independent_field(id, field.getId()));
	}

	Field getFirstObjectiveField()
	{
		return Field(cmzn_optimisation_get_first_objective_field(id));
	}

	Field getNextObjectiveField(const Field& refField)
	{
		return Field(cmzn_optimisation_get_next_objective_field(id, refField.getId()));
	}

	int addObjectiveField(const Field& field)
	{
		return (cmzn_optimisation_add_objective_field(id, field.getId()));
	}

	int removeObjectiveField(const Field& field)
	{
		return (cmzn_optimisation_remove_objective_field(id, field.getId()));
	}

	char *getSolutionReport()
	{
		return cmzn_optimisation_get_solution_report(id);
	}

	int optimise()
	{
		return cmzn_optimisation_optimise(id);
	}

};

inline Optimisation Fieldmodule::createOptimisation()
{
	return Optimisation(cmzn_fieldmodule_create_optimisation(id));
}

}  // namespace Zinc
}

#endif
