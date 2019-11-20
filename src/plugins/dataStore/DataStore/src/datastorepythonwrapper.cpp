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
// Data store Python wrapper
//==============================================================================

#include <Qt>

//==============================================================================
// Note: yes, these two header files must be included in this order...

#include "datastorepythonwrapper.h"
#include "datastorepythonnumpy.h"

//==============================================================================

#include "pythonqtsupport.h"

//==============================================================================

#include <array>

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

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
}

//==============================================================================

using DataStoreValuesDictObject = struct {
                                             PyDictObject mDict;
                                             SimulationSupport::SimulationDataUpdatedFunction *mSimulationDataUpdatedFunction;
                                         };

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
#include "pythonbegin.h"
        auto newValue = PyFloat_AS_DOUBLE(PyNumber_Float(pValue)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
#include "pythonend.h"

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
    nullptr,                                                      // mp_length
    static_cast<binaryfunc>(DataStoreValuesDict_subscript),       // mp_subscript
    static_cast<objobjargproc>(DataStoreValuesDict_ass_subscript) // mp_ass_subscript
};

//==============================================================================

#include "pythonbegin.h"
static PyObject * DataStoreValuesDict_repr(DataStoreValuesDictObject *pValuesDict)
{
    // A string representation of a values dictionary
    // Note: this is a modified version of dict_repr() from dictobject.c in the
    //       Python source code...

    auto mp = reinterpret_cast<PyDictObject *>(pValuesDict);
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
        goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
    }

    while (PyDict_Next(reinterpret_cast<PyObject *>(mp), &i, &key, &value) != 0) {
        PyObject *s;
        int res;

        Py_INCREF(key);
        Py_INCREF(value);

        if (!first) {
            if (_PyUnicodeWriter_WriteASCIIString(&writer, ", ", 2) < 0) {
                goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
            }
        }

        first = false;

        s = PyObject_Repr(key);

        if (s == nullptr) {
            goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        }

        res = _PyUnicodeWriter_WriteStr(&writer, s);

        Py_DECREF(s);

        if (res < 0) {
            goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        }

        if (_PyUnicodeWriter_WriteASCIIString(&writer, ": ", 2) < 0) {
            goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        }

        PythonQtInstanceWrapper *wrappedValue = PythonQtSupport::getInstanceWrapper(value);

        if (wrappedValue != nullptr) {
            auto dataStoreValue = static_cast<DataStoreValue *>(wrappedValue->_objPointerCopy);

            Py_CLEAR(value);

            value = PyFloat_FromDouble(dataStoreValue->value());
        }

        s = PyObject_Repr(value);

        if (s == nullptr) {
            goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        }

        res = _PyUnicodeWriter_WriteStr(&writer, s);

        Py_DECREF(s);

        if (res < 0) {
            goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        }

        Py_CLEAR(key);
        Py_CLEAR(value);
    }

    writer.overallocate = 0;

    if (_PyUnicodeWriter_WriteChar(&writer, '}') < 0) {
        goto error; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
    }

    Py_ReprLeave((PyObject *)mp); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)

    return _PyUnicodeWriter_Finish(&writer);

error:
    Py_ReprLeave((PyObject *)mp); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)
    _PyUnicodeWriter_Dealloc(&writer);

    Py_XDECREF(key);
    Py_XDECREF(value);

    return nullptr;
}
#include "pythonend.h"

//==============================================================================
// Note: a DataStoreValuesDict is a dictionary sub-class for mapping between the
//       values of a DataStoreValues list and Python...

static PyTypeObject DataStoreValuesDict_Type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "OpenCOR.DataStoreValuesDict",                        // tp_name
    sizeof(DataStoreValuesDictObject),                    // tp_basicsize
    0,                                                    // tp_itemsize
    nullptr,                                              // tp_dealloc
    nullptr,                                              // tp_print
    nullptr,                                              // tp_getattr
    nullptr,                                              // tp_setattr
    nullptr,                                              // tp_compare
    reinterpret_cast<reprfunc>(DataStoreValuesDict_repr), // tp_repr
    nullptr,                                              // tp_as_number
    nullptr,                                              // tp_as_sequence
    &DataStoreValuesDict_as_mapping,                      // tp_as_mapping
    nullptr,                                              // tp_hash
    nullptr,                                              // tp_call
    nullptr,                                              // tp_str
    nullptr,                                              // tp_getattro
    nullptr,                                              // tp_setattro
    nullptr,                                              // tp_as_buffer
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,               // tp_flags
    nullptr,                                              // tp_doc
    nullptr,                                              // tp_traverse
    nullptr,                                              // tp_clear
    nullptr,                                              // tp_richcompare
    0,                                                    // tp_weaklistoffset
    nullptr,                                              // tp_iter
    nullptr,                                              // tp_iternext
    nullptr,                                              // tp_methods
    nullptr,                                              // tp_members
    nullptr,                                              // tp_getset
    &PyDict_Type,                                         // tp_base
    nullptr,                                              // tp_dict
    nullptr,                                              // tp_descr_get
    nullptr,                                              // tp_descr_set
    0,                                                    // tp_dictoffset
    nullptr,                                              // tp_init
    nullptr,                                              // tp_alloc
    nullptr,                                              // tp_new
    nullptr,                                              // tp_free
    nullptr,                                              // tp_is_gc
    nullptr,                                              // tp_bases
    nullptr,                                              // tp_mro
    nullptr,                                              // tp_cache
    nullptr,                                              // tp_subclasses
    nullptr,                                              // tp_weaklist
    nullptr,                                              // tp_del
    0,                                                    // tp_version_tag
    nullptr,                                              // tp_finalize
};

//==============================================================================

DataStorePythonWrapper::DataStorePythonWrapper(void *pModule,
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

    // Register some OpenCOR classes with Python and add some decorators to
    // ourselves

    PythonQtSupport::registerClass(&DataStore::staticMetaObject);
    PythonQtSupport::registerClass(&DataStoreValue::staticMetaObject);
    PythonQtSupport::registerClass(&DataStoreVariable::staticMetaObject);

    PythonQtSupport::addInstanceDecorators(this);
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
    // Create and return a NumPy array for the given data store variable and run

    DataStoreArray *dataStoreArray = pDataStoreVariable->array(pRun);

    if ((pDataStoreVariable != nullptr) && (dataStoreArray != nullptr)) {
        auto numPyArray = new NumPyPythonWrapper(dataStoreArray, pDataStoreVariable->size());

        return numPyArray->numPyArray();
    }

#include "pythonbegin.h"
    Py_RETURN_NONE;
#include "pythonend.h"
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

    for (auto dataStoreVariable : pDataStoreVariables) {
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

    std::array<npy_intp, 1> dims = { npy_intp((pSize > 0)?pSize:pDataStoreArray->size()) };

#include "pythonbegin.h"
    mNumPyArray = PyArray_SimpleNewFromData(1, dims.data(), NPY_DOUBLE, static_cast<void *>(mArray->data())); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)

    PyArray_SetBaseObject(reinterpret_cast<PyArrayObject *>(mNumPyArray), // NOLINT(cppcoreguidelines-pro-type-cstyle-cast)
                          PythonQtSupport::wrapQObject(this));
#include "pythonend.h"
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

} // namespace DataStore
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
