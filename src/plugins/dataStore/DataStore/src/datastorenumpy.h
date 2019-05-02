/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

//==============================================================================
// Import the NumPy API
// See https://docs.scipy.org/doc/numpy/reference/c-api.array.html#importing-the-api
//==============================================================================

#pragma once

//==============================================================================

#define NPY_NO_DEPRECATED_API NPY_1_8_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL OpenCOR_Python_Wrapper_PyArray_API

#if defined(__GNUC__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wcast-qual"
	#pragma GCC diagnostic ignored "-Wduplicate-enum"
	#pragma GCC diagnostic ignored "-Wextra-semi"
	#pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
	#pragma GCC diagnostic ignored "-Wold-style-cast"
	#pragma GCC diagnostic ignored "-Wreserved-id-macro"
	#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
	#pragma GCC diagnostic ignored "-Wundef"
#endif

#include <numpy/arrayobject.h>

#if defined(__GNUC__)
	#pragma GCC diagnostic pop
#endif

//==============================================================================
// End of file
//==============================================================================
