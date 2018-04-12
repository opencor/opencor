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
// Python wrapper for DataStore classes
//==============================================================================

#include "datastoreinterface.h"
#include "datastorepythonwrapper.h"  // Needs to come before numpy includes
#include "datastorenumpy.h"
#include "pythonqtsupport.h"

//==============================================================================

#include <PythonQt/PythonQtConversion.h>

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

// We need to wrap import_array() as it is a define that includes a return statement

static bool init_numpy()
{
    import_array1(false);  //  Will return false upon failure
    return true;
}

//==============================================================================

static DataStoreValue *getDataStoreValue(PyObject *valuesDict, PyObject *key)
{
    DataStoreValue *dataStoreValue = 0;

    PyObject *obj = PyDict_GetItem(valuesDict, key);

    if (obj && PyObject_TypeCheck(obj, &PythonQtInstanceWrapper_Type)) {
        PythonQtInstanceWrapper* wrap = (PythonQtInstanceWrapper*)obj;
        dataStoreValue = (DataStoreValue *)wrap->_objPointerCopy;
    }

    return dataStoreValue;
}

//==============================================================================

// Get a subscripted item from a values dictionary

static PyObject *DataStoreValuesDict_subscript(PyObject *valuesDict, PyObject *key)
{
    auto *dataStoreValue = getDataStoreValue(valuesDict, key);

    if (dataStoreValue) {
        return PyFloat_FromDouble(dataStoreValue->value());
    } else {
        Py_RETURN_NONE;
    }
}

//==============================================================================

// Assign to a subscripted item in a values dictionary

static int DataStoreValuesDict_ass_subscript(PyObject *valuesDict, PyObject *key, PyObject *value)
{
    if (value == 0) {
        return PyDict_DelItem(valuesDict, key);
    }

    else if (PyNumber_Check(value)) {
        auto *dataStoreValue = getDataStoreValue(valuesDict, key);

        if (dataStoreValue) {
            auto newValue = PyFloat_AS_DOUBLE(PyNumber_Float(value));

            if (dataStoreValue->value() != newValue) {
                dataStoreValue->setValue(newValue);

                // Let our SimulationData object know that data values have changed

                SimulationSupport::SimulationDataUpdatedFunction *simulationDataUpdatedFunction =
                    ((DataStoreValuesDictObject *)valuesDict)->mSimulationDataUpdatedFunction;
                if (simulationDataUpdatedFunction)
                    (*simulationDataUpdatedFunction)();
            }

            return 0;
        }
    }

    PyErr_SetString(PyExc_TypeError, "Invalid value");
    return -1;
}

//==============================================================================

static PyMappingMethods DataStoreValuesDict_as_mapping = {
    0,                                                 /*mp_length*/
    (binaryfunc)DataStoreValuesDict_subscript,         /*mp_subscript*/
    (objobjargproc)DataStoreValuesDict_ass_subscript,  /*mp_ass_subscript*/
};

//==============================================================================

// A string representation of a values dictionary

static PyObject *DataStoreValuesDict_repr(DataStoreValuesDictObject *valuesDict)
{
    // A modified version of `dict_repr()` from `dictobject.c` in Python's C source

    PyDictObject *mp = (PyDictObject *)valuesDict;

    Py_ssize_t i;
    PyObject *key = NULL, *value = NULL;
    _PyUnicodeWriter writer;
    int first;

    i = Py_ReprEnter((PyObject *)mp);
    if (i != 0) {
        return i > 0 ? PyUnicode_FromString("{...}") : NULL;
    }

    if (mp->ma_used == 0) {
        Py_ReprLeave((PyObject *)mp);
        return PyUnicode_FromString("{}");
    }

    _PyUnicodeWriter_Init(&writer);
    writer.overallocate = 1;
    /* "{" + "1: 2" + ", 3: 4" * (len - 1) + "}" */
    writer.min_length = 1 + 4 + (2 + 4) * (mp->ma_used - 1) + 1;

    if (_PyUnicodeWriter_WriteChar(&writer, '{') < 0)
        goto error;

    /* Do repr() on each key+value pair, and insert ": " between them.
       Note that repr may mutate the dict. */
    i = 0;
    first = 1;
    while (PyDict_Next((PyObject *)mp, &i, &key, &value)) {
        PyObject *s;
        int res;

        /* Prevent repr from deleting key or value during key format. */
        Py_INCREF(key);
        Py_INCREF(value);

        if (!first) {
            if (_PyUnicodeWriter_WriteASCIIString(&writer, ", ", 2) < 0)
                goto error;
        }
        first = 0;

        s = PyObject_Repr(key);
        if (s == NULL)
            goto error;
        res = _PyUnicodeWriter_WriteStr(&writer, s);
        Py_DECREF(s);
        if (res < 0)
            goto error;

        if (_PyUnicodeWriter_WriteASCIIString(&writer, ": ", 2) < 0)
            goto error;

        if (PyObject_TypeCheck(value, &PythonQtInstanceWrapper_Type)) {
            PythonQtInstanceWrapper* wrap = (PythonQtInstanceWrapper*)value;
            DataStoreValue *dataStoreValue = (DataStoreValue *)wrap->_objPointerCopy;
            Py_CLEAR(value);
            value = PyFloat_FromDouble(dataStoreValue->value());
        }

        s = PyObject_Repr(value);
        if (s == NULL)
            goto error;
        res = _PyUnicodeWriter_WriteStr(&writer, s);
        Py_DECREF(s);
        if (res < 0)
            goto error;

        Py_CLEAR(key);
        Py_CLEAR(value);
    }

    writer.overallocate = 0;
    if (_PyUnicodeWriter_WriteChar(&writer, '}') < 0)
        goto error;

    Py_ReprLeave((PyObject *)mp);

    return _PyUnicodeWriter_Finish(&writer);

error:
    Py_ReprLeave((PyObject *)mp);
    _PyUnicodeWriter_Dealloc(&writer);
    Py_XDECREF(key);
    Py_XDECREF(value);
    return NULL;
}

//==============================================================================

#if defined(__GNUC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

// A `DataStoreValuesDict` is a dictionary sub-class for mapping between the values
// of a DataStoreValues list and Python.

PyTypeObject DataStorePythonWrapper::DataStoreValuesDict_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "OpenCOR.DataStoreValuesDict",
    sizeof(DataStoreValuesDictObject),
    0,
    0,                                          /* tp_dealloc */
    0,                                          /* tp_print */
    0,                                          /* tp_getattr */
    0,                                          /* tp_setattr */
    0,                                          /* tp_compare */
    (reprfunc)DataStoreValuesDict_repr,         /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    &DataStoreValuesDict_as_mapping,            /* tp_as_mapping */
    0,                                          /* tp_hash */
    0,                                          /* tp_call */
    0,                                          /* tp_str */
    0,                                          /* tp_getattro */
    0,                                          /* tp_setattro */
    0,                                          /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
    0,                                          /* tp_doc */
    0,                                          /* tp_traverse */
    0,                                          /* tp_clear */
    0,                                          /* tp_richcompare */
    0,                                          /* tp_weaklistoffset */
    0,                                          /* tp_iter */
    0,                                          /* tp_iternext */
    0,                                          /* tp_methods */
    0,                                          /* tp_members */
    0,                                          /* tp_getset */
    &PyDict_Type,                               /* tp_base */
};

#if defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

//==============================================================================

DataStorePythonWrapper::DataStorePythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    Q_UNUSED(pModule);

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == NULL) {
        if (!init_numpy()) qFatal("Unable to initialise NumPy API...");
    }

    PyType_Ready(&DataStoreValuesDict_Type);

    PythonQtSupport::registerClass(&OpenCOR::DataStore::DataStore::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::DataStore::DataStoreValue::staticMetaObject);
    PythonQtSupport::registerClass(&OpenCOR::DataStore::DataStoreVariable::staticMetaObject);

    PythonQtSupport::addInstanceDecorators(this);
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreArray *pDataStoreArray)
{
    if (pDataStoreArray) {
        auto numpyArray = new NumpyPythonWrapper(pDataStoreArray);
        return numpyArray->numpyArray();
    } else {
        Py_RETURN_NONE;
    }
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreVariable *pDataStoreVariable, const int &pRun)
{
    if (pDataStoreVariable && pDataStoreVariable->array(pRun)) {
        auto numpyArray = new NumpyPythonWrapper(pDataStoreVariable->array(pRun), pDataStoreVariable->size());
        return numpyArray->numpyArray();
    } else {
        Py_RETURN_NONE;
    }
}


//==============================================================================

double DataStorePythonWrapper::valueAt(DataStoreVariable *pDataStoreVariable,
                                       const quint64 &pPosition, const int &pRun) const
{
    if (pDataStoreVariable && pDataStoreVariable->array()) {
        return pDataStoreVariable->valueAt(pPosition, pRun);
    } else {
        throw std::runtime_error("'NoneType' object is not subscriptable");
        return 0.0;
    }
}

//==============================================================================

PyObject * DataStorePythonWrapper::values(DataStoreVariable *pDataStoreVariable, const int &pRun) const
{
    return DataStorePythonWrapper::newNumPyArray(pDataStoreVariable, pRun);
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreValuesDict(
    const DataStoreValues *pDataStoreValues,
    SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction)
{
    PyObject *valuesDict = PyDict_Type.tp_new(&DataStoreValuesDict_Type, NULL, NULL);
    valuesDict->ob_type = &DataStoreValuesDict_Type;

    ((DataStoreValuesDictObject *)valuesDict)->mSimulationDataUpdatedFunction = pSimulationDataUpdatedFunction;

    for (int i = 0; i < pDataStoreValues->size(); ++i) {
        DataStoreValue *value = pDataStoreValues->at(i);

        PythonQtSupport::addObject(valuesDict, value->uri(), value);
    }

    return valuesDict;
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreVariablesDict(
    const DataStoreVariables &pDataStoreVariables)
{
    PyObject *variablesDict = PyDict_New();
    foreach (DataStoreVariable *variable, pDataStoreVariables)
        PythonQtSupport::addObject(variablesDict, variable->uri(), variable);

    return variablesDict;
}

//==============================================================================

PyObject * DataStorePythonWrapper::variables(DataStore *pDataStore)
{
    return dataStoreVariablesDict(pDataStore->variables());
}

//==============================================================================

PyObject * DataStorePythonWrapper::voiAndVariables(DataStore *pDataStore)
{
    return dataStoreVariablesDict(pDataStore->voiAndVariables());
}

//==============================================================================

NumpyPythonWrapper::NumpyPythonWrapper(DataStoreArray *pDataStoreArray, quint64 pSize) :
    mArray(pDataStoreArray)
{
    npy_intp dims[1];
    dims[0] = (pSize > 0) ? pSize : pDataStoreArray->size();

    mArray->incRef();
    mNumPyArray = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void *)mArray->data());

    mPythonObject = PythonQtSupport::wrapQObject(this);
    PyArray_SetBaseObject((PyArrayObject *)mNumPyArray, mPythonObject);
}

//==============================================================================

NumpyPythonWrapper::~NumpyPythonWrapper()
{
    mArray->decRef();
}

//==============================================================================

PyObject * NumpyPythonWrapper::numpyArray() const
{
    return mNumPyArray;
}

//==============================================================================

PyObject * NumpyPythonWrapper::pythonObject() const
{
    return mPythonObject;
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
