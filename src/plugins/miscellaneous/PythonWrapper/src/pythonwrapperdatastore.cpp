/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// Python wrapper for DataStore classes
//==============================================================================

#include "datastoreinterface.h"
#include "pythonwrapperdatastore.h"
#include "pythonwrappernumpy.h"
#include "pythonwrapperplugin.h"

//==============================================================================

namespace OpenCOR {
namespace PythonWrapper {

//==============================================================================

// We need to wrap import_array() as it is a define that includes a return statement

static bool init_numpy()
{
    import_array1(false);  //  Will return false upon failure
    return true;
}

//==============================================================================

PythonWrapperDataStore::PythonWrapperDataStore(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    Q_UNUSED(pModule);

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == NULL) {
        if (!init_numpy()) qFatal("Unable to initialise NumPy API...");
    }

    PythonQt::self()->registerClass(&OpenCOR::DataStore::DataStore::staticMetaObject);
    PythonQt::self()->registerClass(&OpenCOR::DataStore::DataStoreVariable::staticMetaObject);

    PythonQt::self()->addInstanceDecorators(this);
}

//==============================================================================

PyObject * PythonWrapperDataStore::newNumPyArray(DataStore::DataStoreArray *pDataStoreArray)
{
    if (pDataStoreArray) {
        auto numpyArray = new PythonWrapperNumPy(pDataStoreArray);
        return numpyArray->numpyArray();
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

PyObject * PythonWrapperDataStore::newNumPyArray(DataStore::DataStoreVariable *pDataStoreVariable)
{
    if (pDataStoreVariable) {
        auto numpyArray = new PythonWrapperNumPy(pDataStoreVariable->array(), pDataStoreVariable->size());
        return numpyArray->numpyArray();
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

PyObject * PythonWrapperDataStore::values(DataStore::DataStoreVariable *pDataStoreVariable) const
{
    return PythonWrapperDataStore::newNumPyArray(pDataStoreVariable);
}

//==============================================================================

PythonWrapperNumPy::PythonWrapperNumPy(DataStore::DataStoreArray *pDataStoreArray, qulonglong pSize) :
    mArray(pDataStoreArray)
{
    npy_intp dims[1];
    dims[0] = (pSize > 0) ? pSize : pDataStoreArray->capacity();

    mArray->incReference();
    mNumPyArray = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void *)mArray->values());

    mPythonObject = PythonQt::priv()->wrapQObject(this);
    PyArray_SetBaseObject((PyArrayObject *)mNumPyArray, mPythonObject);
}

//==============================================================================

PythonWrapperNumPy::~PythonWrapperNumPy()
{
    mArray->decReference();
}

//==============================================================================

PyObject * PythonWrapperNumPy::numpyArray() const
{
    return mNumPyArray;
}

//==============================================================================

PyObject * PythonWrapperNumPy::pythonObject() const
{
    return mPythonObject;
}

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
