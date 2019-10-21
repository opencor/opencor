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

//==============================================================================
// Note: the order in which we include the two following header files is
//       important...

#include "datastorepythonwrapper.h"
#include "datastorepythonnumpy.h"

//==============================================================================

#include "pythonqtsupport.h"

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

static bool initNumPy()
{
    // Initialise NumPy

    import_array1(false)

    return true;
}

//==============================================================================

typedef struct {
    PyDictObject mDict;
    SimulationSupport::SimulationDataUpdatedFunction *mSimulationDataUpdatedFunction;
} DataStoreValuesDictObject;

//==============================================================================

static DataStoreValue * getDataStoreValue(PyObject *pValuesDict, PyObject *pKey)
{
    // Get and return a DataStoreValue item from a values dictionary

    PythonQtInstanceWrapper *wrappedObject = PythonQtSupport::getInstanceWrapper(PyDict_GetItem(pValuesDict, pKey));

    if (wrappedObject != nullptr) {
        return static_cast<DataStoreValue *>(wrappedObject->_objPointerCopy);
    }

    return nullptr;
}

//==============================================================================

static PyObject * DataStoreValuesDict_subscript(PyObject *pValuesDict,
                                                PyObject *pKey)
{
    // Get and return a subscripted item from a values dictionary

    auto *dataStoreValue = getDataStoreValue(pValuesDict, pKey);

    if (dataStoreValue != nullptr) {
        return PyFloat_FromDouble(dataStoreValue->value());
    }

    Py_RETURN_NONE;
}

//==============================================================================

static int DataStoreValuesDict_ass_subscript(PyObject *pValuesDict,
                                             PyObject *pKey, PyObject *pValue)
{
    // Assign to a subscripted item in a values dictionary

    if (pValue == nullptr) {
        return PyDict_DelItem(pValuesDict, pKey);
    }

    PyNumber_Check(pValue);

    auto dataStoreValue = getDataStoreValue(pValuesDict, pKey);

    if (dataStoreValue != nullptr) {
        auto newValue = PyFloat_AS_DOUBLE(PyNumber_Float(pValue));

        if (!qFuzzyCompare(dataStoreValue->value(), newValue)) {
            dataStoreValue->setValue(newValue);

            // Let our SimulationData object know that simulation data values
            // have been updated

            auto simulationDataUpdatedFunction = reinterpret_cast<DataStoreValuesDictObject *>(pValuesDict)->mSimulationDataUpdatedFunction;

            if (simulationDataUpdatedFunction != nullptr) {
                (*simulationDataUpdatedFunction)();
            }
        }

        return 0;
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

static PyObject * DataStoreValuesDict_repr(DataStoreValuesDictObject *pValuesDict)
{
    // A string representation of a values dictionary
    // Note: this is a modified version of dict_repr() from dictobject.c in
    //       Python's C source code...

    PyDictObject *mp = reinterpret_cast<PyDictObject *>(pValuesDict);
    Py_ssize_t i = Py_ReprEnter(reinterpret_cast<PyObject *>(mp));
    PyObject *key = nullptr;
    PyObject *value = nullptr;
    bool first = true;

    if (i != 0) {
        return (i > 0)?
                    PyUnicode_FromString("{...}"):
                    nullptr;
    }

    if (mp->ma_used == 0) {
        Py_ReprLeave(reinterpret_cast<PyObject *>(mp));

        return PyUnicode_FromString("{}");
    }

    _PyUnicodeWriter writer;

    _PyUnicodeWriter_Init(&writer);

    writer.overallocate = 1;
    writer.min_length = 1+4+(2+4)*(mp->ma_used-1)+1;

    if (_PyUnicodeWriter_WriteChar(&writer, '{') < 0) {
        goto error;
    }

    while (PyDict_Next(reinterpret_cast<PyObject *>(mp), &i, &key, &value)) {
        PyObject *s;
        int res;

        Py_INCREF(key);
        Py_INCREF(value);

        if (!first) {
            if (_PyUnicodeWriter_WriteASCIIString(&writer, ", ", 2) < 0) {
                goto error;
            }
        }

        first = false;

        s = PyObject_Repr(key);

        if (s == nullptr) {
            goto error;
        }

        res = _PyUnicodeWriter_WriteStr(&writer, s);

        Py_DECREF(s);

        if (res < 0) {
            goto error;
        }

        if (_PyUnicodeWriter_WriteASCIIString(&writer, ": ", 2) < 0) {
            goto error;
        }

        auto wrappedValue = PythonQtSupport::getInstanceWrapper(value);

        if (wrappedValue != nullptr) {
            auto dataStoreValue = static_cast<DataStoreValue *>(wrappedValue->_objPointerCopy);

            Py_CLEAR(value);

            value = PyFloat_FromDouble(dataStoreValue->value());
        }

        s = PyObject_Repr(value);

        if (s == nullptr) {
            goto error;
        }

        res = _PyUnicodeWriter_WriteStr(&writer, s);

        Py_DECREF(s);

        if (res < 0) {
            goto error;
        }

        Py_CLEAR(key);
        Py_CLEAR(value);
    }

    writer.overallocate = 0;

    if (_PyUnicodeWriter_WriteChar(&writer, '}') < 0) {
        goto error;
    }

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
// Note: a DataStoreValuesDict is a dictionary sub-class for mapping between the
//       values of a DataStoreValues list and Python...

PyTypeObject DataStorePythonWrapper::DataStoreValuesDict_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "OpenCOR.DataStoreValuesDict",                          // tp_name
    sizeof(DataStoreValuesDictObject),                      // tp_basicsize
    0,                                                      // tp_itemsize
    nullptr,                                                // tp_dealloc
    nullptr,                                                // tp_print
    nullptr,                                                // tp_getattr
    nullptr,                                                // tp_setattr
    nullptr,                                                // tp_compare
    reinterpret_cast<reprfunc>(DataStoreValuesDict_repr),   // tp_repr
    nullptr,                                                // tp_as_number
    nullptr,                                                // tp_as_sequence
    &DataStoreValuesDict_as_mapping,                        // tp_as_mapping
    nullptr,                                                // tp_hash
    nullptr,                                                // tp_call
    nullptr,                                                // tp_str
    nullptr,                                                // tp_getattro
    nullptr,                                                // tp_setattro
    nullptr,                                                // tp_as_buffer
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,                 // tp_flags
    nullptr,                                                // tp_doc
    nullptr,                                                // tp_traverse
    nullptr,                                                // tp_clear
    nullptr,                                                // tp_richcompare
    0,                                                      // tp_weaklistoffset
    nullptr,                                                // tp_iter
    nullptr,                                                // tp_iternext
    nullptr,                                                // tp_methods
    nullptr,                                                // tp_members
    nullptr,                                                // tp_getset
    &PyDict_Type                                            // tp_base
};

//==============================================================================

DataStorePythonWrapper::DataStorePythonWrapper(PyObject *pModule,
                                               QObject *pParent) :
    QObject(pParent)
{
    Q_UNUSED(pModule)

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == nullptr) {
#ifdef QT_DEBUG
        bool res =
#endif
        initNumPy();

#ifdef QT_DEBUG
        if (!res) {
            qFatal("FATAL ERROR | %s:%d: unable to initialise NumPy.", __FILE__, __LINE__);
        }
#endif
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
    // Create and return a NumPy array for the given data store array

    if (pDataStoreArray != nullptr) {
        auto numPyArray = new NumPyPythonWrapper(pDataStoreArray);

        return numPyArray->numPyArray();
    }

    Py_RETURN_NONE;
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreVariable *pDataStoreVariable,
                                                 int pRun)
{
    // Create and return a NumPy array for the given data store variable and run

    DataStoreArray *dataStoreArray = pDataStoreVariable->array(pRun);

    if ((pDataStoreVariable != nullptr) && (dataStoreArray != nullptr)) {
        auto numPyArray = new NumPyPythonWrapper(dataStoreArray, pDataStoreVariable->size());

        return numPyArray->numPyArray();
    }

    Py_RETURN_NONE;
}


//==============================================================================

double DataStorePythonWrapper::value(DataStoreVariable *pDataStoreVariable,
                                     quint64 pPosition, int pRun) const
{
    // Return the value of the given data store variable at the given position
    // and for the given run

    if (   (pDataStoreVariable != nullptr)
        && (pDataStoreVariable->array() != nullptr)) {
        return pDataStoreVariable->value(pPosition, pRun);
    }

    throw std::runtime_error("'NoneType' object is not subscriptable.");
}

//==============================================================================

PyObject * DataStorePythonWrapper::values(DataStoreVariable *pDataStoreVariable,
                                          int pRun) const
{
    // Return the values of the given data store viarable for the given run as a
    // NumPy array

    return DataStorePythonWrapper::newNumPyArray(pDataStoreVariable, pRun);
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreValuesDict(const DataStoreValues *pDataStoreValues,
                                                       SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction)
{
    // Create and return a Python dictionary for the given data store values and
    // keep track of the given simulation data updated function so that we can
    // let OpenCOR know when simulation data have been updated

    PyObject *res = PyDict_Type.tp_new(&DataStoreValuesDict_Type, nullptr, nullptr);

    res->ob_type = &DataStoreValuesDict_Type;

    reinterpret_cast<DataStoreValuesDictObject *>(res)->mSimulationDataUpdatedFunction = pSimulationDataUpdatedFunction;

    if (pDataStoreValues != nullptr) {
        for (int i = 0, iMax = pDataStoreValues->size(); i < iMax; ++i) {
            auto value = pDataStoreValues->at(i);

            PythonQtSupport::addObject(res, value->uri(), value);
        }
    }

    return res;
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreVariablesDict(const DataStoreVariables &pDataStoreVariables)
{
    // Create and return a Python dictionary for the given data store variables

    PyObject *res = PyDict_New();

    foreach (DataStoreVariable *dataStoreVariable, pDataStoreVariables) {
        PythonQtSupport::addObject(res, dataStoreVariable->uri(), dataStoreVariable);
    }

    return res;
}

//==============================================================================

PyObject * DataStorePythonWrapper::variables(DataStore *pDataStore)
{
    // Return the variables in the given data store as a Python dictionary

    return dataStoreVariablesDict(pDataStore->variables());
}

//==============================================================================

PyObject * DataStorePythonWrapper::voiAndVariables(DataStore *pDataStore)
{
    // Return the VOI and variables in the given data store as a Python
    // dictionary

    return dataStoreVariablesDict(pDataStore->voiAndVariables());
}

//==============================================================================

NumPyPythonWrapper::NumPyPythonWrapper(DataStoreArray *pDataStoreArray,
                                       quint64 pSize) :
    mArray(pDataStoreArray)
{
    // Tell our array that we are holding it

    mArray->hold();

    // Initialise ourselves

    npy_intp dims[1] = { npy_intp((pSize > 0)?pSize:pDataStoreArray->size()) };

    mNumPyArray = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, static_cast<void *>(mArray->data()));
    mPythonObject = PythonQtSupport::wrapQObject(this);

    PyArray_SetBaseObject(reinterpret_cast<PyArrayObject *>(mNumPyArray), mPythonObject);
}

//==============================================================================

NumPyPythonWrapper::~NumPyPythonWrapper()
{
    // Tell our array that we are releasing it

    mArray->release();
}

//==============================================================================

PyObject * NumPyPythonWrapper::numPyArray() const
{
    // Return our NumPy array

    return mNumPyArray;
}

//==============================================================================

PyObject * NumPyPythonWrapper::pythonObject() const
{
    // Return our Python object

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
