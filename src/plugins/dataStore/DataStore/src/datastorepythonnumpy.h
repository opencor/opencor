/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/

//==============================================================================
// Import the NumPy API
//==============================================================================

#pragma once

//==============================================================================
// Note: see https://docs.scipy.org/doc/numpy/reference/c-api.array.html#importing-the-api...

#define NPY_NO_DEPRECATED_API NPY_1_8_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL OpenCOR_Python_Wrapper_PyArray_API

//==============================================================================

#include "pythonbegin.h"
    #include <numpy/arrayobject.h>
#include "pythonend.h"

//==============================================================================
// End of file
//==============================================================================
