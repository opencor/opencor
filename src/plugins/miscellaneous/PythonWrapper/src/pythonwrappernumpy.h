/*******************************************************************************

Copyright The University of Auckland

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************/

//==============================================================================
// Import the NumPy API
// See https://docs.scipy.org/doc/numpy/reference/c-api.array.html#importing-the-api
//==============================================================================

#pragma once

//==============================================================================

#define NPY_NO_DEPRECATED_API NPY_1_8_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL OpenCOR_Python_Wrapper_PyArray_API

#include <numpy/arrayobject.h>

//==============================================================================
// End of file
//==============================================================================
