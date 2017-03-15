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
#include "pythonqtconversion.h"
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

static DataStore::DataStoreVariable *getVariable(PyDictObject *valuesDict, PyObject *key)
{
    DataStore::DataStoreVariable *variable = 0;

    PyObject *obj = PyDict_GetItem((PyObject *)valuesDict, key);

    if (obj && PyObject_TypeCheck(obj, &PythonQtInstanceWrapper_Type)) {
        PythonQtInstanceWrapper* wrap = (PythonQtInstanceWrapper*)obj;
        variable = (DataStore::DataStoreVariable *)wrap->_objPointerCopy;
    }

    return variable;
}

//==============================================================================

static PyObject *valuesdict_subscript(PyDictObject *valuesDict, PyObject *key)
{
    DataStore::DataStoreVariable *variable = getVariable(valuesDict, key);

    if (variable) {
        return PyFloat_FromDouble(variable->nextValue());
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

//==============================================================================

static int valuesdict_ass_subscript(PyDictObject *valuesDict, PyObject *key, PyObject *value)
{
    if (value == 0) {
        return PyDict_DelItem((PyObject *)valuesDict, key);
    }

    else if (PyNumber_Check(value)) {
        DataStore::DataStoreVariable *variable = getVariable(valuesDict, key);

        if (variable) {
            variable->setNextValue(PyFloat_AS_DOUBLE(PyNumber_Float(value)));
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

static PyObject* valuesdict_repr(PyDictObject *valuesDict)
{
    // A modified version of `dict_repr()` from `dictobject.c` in Python's C source

    PyDictObject *mp = valuesDict;

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
            DataStore::DataStoreVariable *variable = (DataStore::DataStoreVariable *)wrap->_objPointerCopy;
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

// A `valuesdict` is a dictionary sub-class for mapping between the next values
// of a DataStoreVariables list and Python.

static PyTypeObject valuesdict_type = {
    PyVarObject_HEAD_INIT(&PyType_Type, 0)
    "OpenCOR.valuesdict",
    sizeof(PyDictObject),
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
//    PyDict_Type.tp_init,                        /* tp_init */
//    PyType_GenericAlloc,                        /* tp_alloc */
//    PyDict_Type.tp_new,                         /* tp_new */
//    PyObject_GC_Del,                            /* tp_free */
};

//==============================================================================

PythonWrapperDataStore::PythonWrapperDataStore(PyObject *pModule, QObject *pParent) : QObject(pParent)
{
    Q_UNUSED(pModule);

    // Initialise NumPy

    if (OpenCOR_Python_Wrapper_PyArray_API == NULL) {
        if (!init_numpy()) qFatal("Unable to initialise NumPy API...");
    }

    PyType_Ready(&valuesdict_type);

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

PyObject * PythonWrapperDataStore::dataStoreValuesDict(
    const DataStore::DataStoreVariables &pDataStoreVariables)
{
    PyObject *valuesDict = PyDict_New();
    valuesDict->ob_type = &valuesdict_type;

    foreach (DataStore::DataStoreVariable *variable, pDataStoreVariables)
        PythonQt::self()->addObject(valuesDict, variable->uri(), variable);

    return valuesDict;
}

//==============================================================================

PyObject * PythonWrapperDataStore::dataStoreVariablesDict(
    const DataStore::DataStoreVariables &pDataStoreVariables)
{
    PyObject *variablesDict = PyDict_New();
    foreach (DataStore::DataStoreVariable *variable, pDataStoreVariables)
        PythonQt::self()->addObject(variablesDict, variable->uri(), variable);

    return variablesDict;
}

//==============================================================================

PyObject * PythonWrapperDataStore::variables(DataStore::DataStore *pDataStore)
{
    return dataStoreVariablesDict(pDataStore->variables());
}

//==============================================================================

PyObject * PythonWrapperDataStore::voiAndVariables(DataStore::DataStore *pDataStore)
{
    return dataStoreVariablesDict(pDataStore->voiAndVariables());
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
