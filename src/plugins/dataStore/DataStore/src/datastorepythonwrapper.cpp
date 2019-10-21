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

#include <Qt>

//==============================================================================

#include "pythonbegin.h"

#include "datastorepythonwrapper.h"  // Needs to come before numpy includes
#include "datastorepythonnumpy.h"

#include "pythonqtsupport.h"

#include "PythonQt/PythonQtConversion.h"

//==============================================================================

#include <stdexcept>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

static bool init_numpy()
{
    import_array1(false)

    return true;
}

//==============================================================================

static DataStoreValue *getDataStoreValue(PyObject *valuesDict, PyObject *key)
{
    DataStoreValue *dataStoreValue = nullptr;

    PyObject *object = PyDict_GetItem(valuesDict, key);
    PythonQtInstanceWrapper *wrappedObject = PythonQtSupport::getInstanceWrapper(object);

    if (wrappedObject) {
        dataStoreValue = (DataStoreValue *)wrappedObject->_objPointerCopy;
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

typedef struct {
    PyDictObject mDict;
    SimulationSupport::SimulationDataUpdatedFunction *mSimulationDataUpdatedFunction;
} DataStoreValuesDictObject;

//==============================================================================

// Assign to a subscripted item in a values dictionary

static int DataStoreValuesDict_ass_subscript(PyObject *valuesDict, PyObject *key, PyObject *value)
{
    if (value == nullptr) {
        return PyDict_DelItem(valuesDict, key);
    }

    else if (PyNumber_Check(value)) {
        auto *dataStoreValue = getDataStoreValue(valuesDict, key);

        if (dataStoreValue) {
            auto newValue = PyFloat_AS_DOUBLE(PyNumber_Float(value));
#if defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
            if (dataStoreValue->value() != newValue) {
                dataStoreValue->setValue(newValue);
#if defined(__GNUC__)
    #pragma GCC diagnostic pop
#endif
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
    nullptr,                                                        // mp_length
    static_cast<binaryfunc>(DataStoreValuesDict_subscript),         // mp_subscript
    static_cast<objobjargproc>(DataStoreValuesDict_ass_subscript)   // mp_ass_subscript
};

//==============================================================================

// A string representation of a values dictionary

static PyObject *DataStoreValuesDict_repr(DataStoreValuesDictObject *valuesDict)
{
    // A modified version of `dict_repr()` from `dictobject.c` in Python's C source

    PyDictObject *mp = (PyDictObject *)valuesDict;

    Py_ssize_t i;
    PyObject *key = nullptr, *value = nullptr;
    _PyUnicodeWriter writer;
    int first;

    i = Py_ReprEnter((PyObject *)mp);
    if (i != 0) {
        return i > 0 ? PyUnicode_FromString("{...}") : nullptr;
    }

    if (mp->ma_used == 0) {
        Py_ReprLeave((PyObject *)mp);
        return PyUnicode_FromString("{}");
    }

    _PyUnicodeWriter_Init(&writer);
    writer.overallocate = 1;
    // "{" + "1: 2" + ", 3: 4" * (len - 1) + "}"
    writer.min_length = 1 + 4 + (2 + 4) * (mp->ma_used - 1) + 1;

    if (_PyUnicodeWriter_WriteChar(&writer, '{') < 0)
        goto error;

    // Do repr() on each key+value pair, and insert ": " between them.
    // Note: repr() may mutate the dictionary...

    i = 0;
    first = 1;
    while (PyDict_Next((PyObject *)mp, &i, &key, &value)) {
        PyObject *s;
        int res;

        // Prevent repr() from deleting key or value during key format

        Py_INCREF(key);
        Py_INCREF(value);

        if (!first) {
            if (_PyUnicodeWriter_WriteASCIIString(&writer, ", ", 2) < 0)
                goto error;
        }
        first = 0;

        s = PyObject_Repr(key);
        if (s == nullptr)
            goto error;
        res = _PyUnicodeWriter_WriteStr(&writer, s);
        Py_DECREF(s);
        if (res < 0)
            goto error;

        if (_PyUnicodeWriter_WriteASCIIString(&writer, ": ", 2) < 0)
            goto error;

        PythonQtInstanceWrapper *wrappedValue = PythonQtSupport::getInstanceWrapper(value);

        if (wrappedValue) {
            DataStoreValue *dataStoreValue = (DataStoreValue *)wrappedValue->_objPointerCopy;
            Py_CLEAR(value);
            value = PyFloat_FromDouble(dataStoreValue->value());
        }

        s = PyObject_Repr(value);
        if (s == nullptr)
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
    return nullptr;
}

//==============================================================================

// A `DataStoreValuesDict` is a dictionary sub-class for mapping between the
// values of a DataStoreValues list and Python

PyTypeObject DataStorePythonWrapper::DataStoreValuesDict_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "OpenCOR.DataStoreValuesDict",            // tp_name
    sizeof(DataStoreValuesDictObject),        // tp_basicsize
    0,                                        // tp_itemsize
    nullptr,                                  // tp_dealloc
    nullptr,                                  // tp_print
    nullptr,                                  // tp_getattr
    nullptr,                                  // tp_setattr
    nullptr,                                  // tp_compare
    (reprfunc) DataStoreValuesDict_repr,      // tp_repr
    nullptr,                                  // tp_as_number
    nullptr,                                  // tp_as_sequence
    &DataStoreValuesDict_as_mapping,          // tp_as_mapping
    nullptr,                                  // tp_hash
    nullptr,                                  // tp_call
    nullptr,                                  // tp_str
    nullptr,                                  // tp_getattro
    nullptr,                                  // tp_setattro
    nullptr,                                  // tp_as_buffer
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,   // tp_flags
    nullptr,                                  // tp_doc
    nullptr,                                  // tp_traverse
    nullptr,                                  // tp_clear
    nullptr,                                  // tp_richcompare
    0,                                        // tp_weaklistoffset
    nullptr,                                  // tp_iter
    nullptr,                                  // tp_iternext
    nullptr,                                  // tp_methods
    nullptr,                                  // tp_members
    nullptr,                                  // tp_getset
    &PyDict_Type                              // tp_base
};

//==============================================================================

DataStorePythonWrapper::DataStorePythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    Q_UNUSED(pModule)

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == nullptr) {
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
    }

    Py_RETURN_NONE;
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreVariable *pDataStoreVariable,
                                                 int pRun)
{
    if (pDataStoreVariable && pDataStoreVariable->array(pRun)) {
        auto numpyArray = new NumpyPythonWrapper(pDataStoreVariable->array(pRun), pDataStoreVariable->size());

        return numpyArray->numpyArray();
    }

    Py_RETURN_NONE;
}


//==============================================================================

double DataStorePythonWrapper::value(DataStoreVariable *pDataStoreVariable,
                                     const quint64 &pPosition, int pRun) const
{
    if (pDataStoreVariable && pDataStoreVariable->array()) {
        return pDataStoreVariable->value(pPosition, pRun);
    }

    throw std::runtime_error("'NoneType' object is not subscriptable");
}

//==============================================================================

PyObject * DataStorePythonWrapper::values(DataStoreVariable *pDataStoreVariable,
                                          int pRun) const
{
    return DataStorePythonWrapper::newNumPyArray(pDataStoreVariable, pRun);
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreValuesDict(
    const DataStoreValues *pDataStoreValues,
    SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction)
{
    PyObject *valuesDict = PyDict_Type.tp_new(&DataStoreValuesDict_Type, nullptr, nullptr);
    valuesDict->ob_type = &DataStoreValuesDict_Type;

    ((DataStoreValuesDictObject *)valuesDict)->mSimulationDataUpdatedFunction = pSimulationDataUpdatedFunction;

    if (pDataStoreValues != nullptr) {
        for (int i = 0; i < pDataStoreValues->size(); ++i) {
            DataStoreValue *value = pDataStoreValues->at(i);

            PythonQtSupport::addObject(valuesDict, value->uri(), value);
        }
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
    npy_intp dims[1] = { npy_intp((pSize > 0)?pSize:pDataStoreArray->size()) };

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

#include "pythonend.h"

//==============================================================================
// End of file
//==============================================================================
