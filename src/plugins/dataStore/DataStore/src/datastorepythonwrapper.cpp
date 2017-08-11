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
#include "pythonsupport.h"

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

static DataStoreVariable *getVariable(PyObject *valuesDict, PyObject *key)
{
    DataStoreVariable *variable = 0;

    PyObject *obj = PyDict_GetItem(valuesDict, key);

    if (obj && PyObject_TypeCheck(obj, &PythonQtInstanceWrapper_Type)) {
        PythonQtInstanceWrapper* wrap = (PythonQtInstanceWrapper*)obj;
        variable = (DataStoreVariable *)wrap->_objPointerCopy;
    }

    return variable;
}

//==============================================================================

// Get a subscripted item from a values dictionary

static PyObject *valuesdict_subscript(PyObject *valuesDict, PyObject *key)
{
    DataStoreVariable *variable = getVariable(valuesDict, key);

    if (variable) {
        return PyFloat_FromDouble(variable->nextValue());
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

// Assign to a subscripted item in a values dictionary

static int valuesdict_ass_subscript(PyObject *valuesDict, PyObject *key, PyObject *value)
{
    if (value == 0) {
        return PyDict_DelItem(valuesDict, key);
    }

    else if (PyNumber_Check(value)) {
        DataStoreVariable *variable = getVariable(valuesDict, key);

        if (variable) {
            double newValue = PyFloat_AS_DOUBLE(PyNumber_Float(value));

            if (variable->nextValue() != newValue) {
                variable->setNextValue(newValue);

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

static PyMappingMethods valuesdict_as_mapping = {
    0,                                          /*mp_length*/
    (binaryfunc)valuesdict_subscript,           /*mp_subscript*/
    (objobjargproc)valuesdict_ass_subscript,    /*mp_ass_subscript*/
};

//==============================================================================

// A string representation of a values dictionary

static PyObject *valuesdict_repr(DataStoreValuesDictObject *valuesDict)
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
            DataStoreVariable *variable = (DataStoreVariable *)wrap->_objPointerCopy;
            Py_CLEAR(value);
            value = PyFloat_FromDouble(variable->nextValue());
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

// A `valuesdict` is a dictionary sub-class for mapping between the next values
// of a DataStoreVariables list and Python.

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
    (reprfunc)valuesdict_repr,                  /* tp_repr */
    0,                                          /* tp_as_number */
    0,                                          /* tp_as_sequence */
    &valuesdict_as_mapping,                     /* tp_as_mapping */
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
//    0,                                          /* tp_dict */
//    0,                                          /* tp_descr_get */
//    0,                                          /* tp_descr_set */
//    0,                                          /* tp_dictoffset */
//    0,                                          /* tp_init */
//    PyType_GenericAlloc,                        /* tp_alloc */
//    PyDict_Type.tp_new,                         /* tp_new */
//    PyObject_GC_Del,                            /* tp_free */
};

#pragma GCC diagnostic pop

//==============================================================================

DataStorePythonWrapper::DataStorePythonWrapper(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    Q_UNUSED(pModule);

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == NULL) {
        if (!init_numpy()) qFatal("Unable to initialise NumPy API...");
    }

    PyType_Ready(&DataStoreValuesDict_Type);

    PythonSupport::registerClass(&OpenCOR::DataStore::DataStore::staticMetaObject);
    PythonSupport::registerClass(&OpenCOR::DataStore::DataStoreVariable::staticMetaObject);

    PythonSupport::addInstanceDecorators(this);
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreArray *pDataStoreArray)
{
    if (pDataStoreArray) {
        auto numpyArray = new NumPyPythonWrapper(pDataStoreArray);
        return numpyArray->numpyArray();
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

PyObject * DataStorePythonWrapper::newNumPyArray(DataStoreVariable *pDataStoreVariable)
{
    if (pDataStoreVariable && pDataStoreVariable->array()) {
        auto numpyArray = new NumPyPythonWrapper(pDataStoreVariable->array(), pDataStoreVariable->size());
        return numpyArray->numpyArray();
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}


//==============================================================================

double DataStorePythonWrapper::value(DataStoreVariable *pDataStoreVariable, const qulonglong &pPosition) const
{
    if (pDataStoreVariable && pDataStoreVariable->array()) {
        return pDataStoreVariable->value(pPosition);
    } else {
        throw std::runtime_error("'NoneType' object is not subscriptable");
        return 0.0;
    }
}

//==============================================================================

PyObject * DataStorePythonWrapper::values(DataStoreVariable *pDataStoreVariable) const
{
    return DataStorePythonWrapper::newNumPyArray(pDataStoreVariable);
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreValuesDict(
    const DataStoreVariables &pDataStoreVariables,
    SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction)
{
    PyObject *valuesDict = PyDict_Type.tp_new(&DataStoreValuesDict_Type, NULL, NULL);
    valuesDict->ob_type = &DataStoreValuesDict_Type;

    ((DataStoreValuesDictObject *)valuesDict)->mSimulationDataUpdatedFunction = pSimulationDataUpdatedFunction;

    foreach (DataStoreVariable *variable, pDataStoreVariables)
        PythonSupport::addObject(valuesDict, variable->uri(), variable);

    return valuesDict;
}

//==============================================================================

PyObject * DataStorePythonWrapper::dataStoreVariablesDict(
    const DataStoreVariables &pDataStoreVariables)
{
    PyObject *variablesDict = PyDict_New();
    foreach (DataStoreVariable *variable, pDataStoreVariables)
        PythonSupport::addObject(variablesDict, variable->uri(), variable);

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

NumPyPythonWrapper::NumPyPythonWrapper(DataStoreArray *pDataStoreArray, qulonglong pSize) :
    mArray(pDataStoreArray)
{
    npy_intp dims[1];
    dims[0] = (pSize > 0) ? pSize : pDataStoreArray->capacity();

    mArray->incReference();
    mNumPyArray = PyArray_SimpleNewFromData(1, dims, NPY_DOUBLE, (void *)mArray->values());

    mPythonObject = PythonSupport::wrapQObject(this);
    PyArray_SetBaseObject((PyArrayObject *)mNumPyArray, mPythonObject);
}

//==============================================================================

NumPyPythonWrapper::~NumPyPythonWrapper()
{
    mArray->decReference();
}

//==============================================================================

PyObject * NumPyPythonWrapper::numpyArray() const
{
    return mNumPyArray;
}

//==============================================================================

PyObject * NumPyPythonWrapper::pythonObject() const
{
    return mPythonObject;
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
