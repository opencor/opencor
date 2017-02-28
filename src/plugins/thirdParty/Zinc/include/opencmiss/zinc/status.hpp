/**
 * @file status.hpp
 *
 * C++ function status/error codes.
 *
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CMZN_STATUS_HPP
#define CMZN_STATUS_HPP

#include "opencmiss/zinc/status.h"

namespace OpenCMISS
{
namespace Zinc
{

enum Status
{
	ERROR_ARGUMENT_CONTEXT = CMZN_ERROR_ARGUMENT_CONTEXT,
	ERROR_NOT_IMPLEMENTED = CMZN_ERROR_NOT_IMPLEMENTED,
	ERROR_INCOMPATIBLE_DATA = CMZN_ERROR_INCOMPATIBLE_DATA,
	ERROR_ALREADY_EXISTS = CMZN_ERROR_ALREADY_EXISTS,
	ERROR_NOT_FOUND = CMZN_ERROR_NOT_FOUND,
	ERROR_MEMORY = CMZN_ERROR_MEMORY,
	ERROR_ARGUMENT = CMZN_ERROR_ARGUMENT,
	ERROR_GENERAL = CMZN_ERROR_GENERAL,
	OK = CMZN_OK
};

} // namespace Zinc
}

#endif
