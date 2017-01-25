# This file is copied from Avogadro
# Home page at http://avogadro.openmolecules.net
# Code repository at https://github.com/cryos/avogadro


# - Find numpy
# Find the native numpy includes
# This module defines
#  NUMPY_INCLUDE_DIR, where to find numpy/arrayobject.h, etc.
#  NUMPY_FOUND, If false, do not try to use numpy headers.

#if (NUMPY_INCLUDE_DIR)
  # in cache already
#  set (NUMPY_FIND_QUIETLY TRUE)
#endif (NUMPY_INCLUDE_DIR)

# Finding NumPy involves calling the Python interpreter
if(NumPy_FIND_REQUIRED)
    find_package(PythonInterp REQUIRED)
else()
    find_package(PythonInterp)
endif()

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c
    "import numpy; print(numpy.get_include())"
    OUTPUT_VARIABLE INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)

if (INCLUDE_DIR)
message (STATUS "Numpy directory: ${INCLUDE_DIR}")
  string(REGEX REPLACE "\\\\" "/" NUMPY_INCLUDE_DIR ${INCLUDE_DIR})
message (STATUS "Numpy directory: ${NUMPY_INCLUDE_DIR}")
  if(EXISTS ${NUMPY_INCLUDE_DIR}/numpy/arrayobject.h)
    # successful
    set (NUMPY_FOUND TRUE)
    set (NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} CACHE STRING "Numpy include path")
  else()
    set(NUMPY_FOUND FALSE)
  endif()
else (INCLUDE_DIR)
  # Did not successfully include numpy
  set(NUMPY_FOUND FALSE)
endif (INCLUDE_DIR)

if (NUMPY_FOUND)
  if (NOT NumPy_FIND_QUIETLY)
    message (STATUS "Numpy headers found at ${NUMPY_INCLUDE_DIR}")
  endif (NOT NumPy_FIND_QUIETLY)
else (NUMPY_FOUND)
  if (NumPy_FIND_REQUIRED)
    message (FATAL_ERROR "Numpy headers missing")
  endif (NumPy_FIND_REQUIRED)
endif (NUMPY_FOUND)

MARK_AS_ADVANCED (NUMPY_INCLUDE_DIR)
