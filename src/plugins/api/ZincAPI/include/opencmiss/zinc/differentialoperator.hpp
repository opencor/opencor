/**
 * @file differentialoperator.hpp
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef CMZN_DIFFFERENTIALOPERATOR_HPP__
#define CMZN_DIFFFERENTIALOPERATOR_HPP__

#include "opencmiss/zinc/differentialoperator.h"

namespace OpenCMISS
{
namespace Zinc
{

class Differentialoperator
{
private:
	cmzn_differentialoperator_id id;

public:

	Differentialoperator() : id(0)
	{  }

	// takes ownership of C handle, responsibility for destroying it
	explicit Differentialoperator(cmzn_differentialoperator_id in_differential_operator_id) :
		id(in_differential_operator_id)
	{  }

	Differentialoperator(const Differentialoperator& differentialOperator) :
		id(cmzn_differentialoperator_access(differentialOperator.id))
	{	}

	Differentialoperator& operator=(const Differentialoperator& differentialOperator)
	{
		cmzn_differentialoperator_id temp_id = cmzn_differentialoperator_access(differentialOperator.id);
		if (0 != id)
		{
			cmzn_differentialoperator_destroy(&id);
		}
		id = temp_id;
		return *this;
	}

	~Differentialoperator()
	{
		if (0 != id)
		{
			cmzn_differentialoperator_destroy(&id);
		}
	}

	bool isValid() const
	{
		return (0 != id);
	}

	cmzn_differentialoperator_id getId() const
	{
		return id;
	}
};

}  // namespace Zinc
}

#endif /* CMZN_DIFFFERENTIAL_OPERATOR_HPP__ */
