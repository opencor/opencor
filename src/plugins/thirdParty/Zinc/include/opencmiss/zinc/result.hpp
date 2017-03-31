/**
 * @file result.hpp
 *
 * Zinc C++ API function result/error codes.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_RESULT_HPP
#define CMZN_RESULT_HPP

#include "opencmiss/zinc/result.h"

namespace OpenCMISS
{
namespace Zinc
{

enum Result
{
	RESULT_ERROR_IN_USE = CMZN_RESULT_ERROR_IN_USE,
	RESULT_ERROR_ARGUMENT_CONTEXT = CMZN_RESULT_ERROR_ARGUMENT_CONTEXT,
	RESULT_ERROR_NOT_IMPLEMENTED = CMZN_RESULT_ERROR_NOT_IMPLEMENTED,
	RESULT_ERROR_INCOMPATIBLE_DATA = CMZN_RESULT_ERROR_INCOMPATIBLE_DATA,
	RESULT_ERROR_ALREADY_EXISTS = CMZN_RESULT_ERROR_ALREADY_EXISTS,
	RESULT_ERROR_NOT_FOUND = CMZN_RESULT_ERROR_NOT_FOUND,
	RESULT_ERROR_MEMORY = CMZN_RESULT_ERROR_MEMORY,
	RESULT_ERROR_ARGUMENT = CMZN_RESULT_ERROR_ARGUMENT,
	RESULT_ERROR_GENERAL = CMZN_RESULT_ERROR_GENERAL,
	RESULT_OK = CMZN_RESULT_OK
};

} // namespace Zinc
}

#endif
