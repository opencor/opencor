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
// Python wrapper for DataStore classes
//==============================================================================

#pragma once

//==============================================================================

#include "PythonQtPythonInclude.h"

//==============================================================================

#include <QList>
#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace DataStore {
    class DataStoreArray;
    class DataStoreVariable;
};

//==============================================================================

namespace PythonWrapper {

//==============================================================================

class PythonWrapperDataStore : public QObject
{
    Q_OBJECT

public:
    void initialise(PyObject *pModule);
    static PyObject * newNumPyArray(DataStore::DataStoreArray *pDataStoreArray);
    static PyObject * newNumPyArray(DataStore::DataStoreVariable *pDataStoreVariable);

private:
    PyObject * variablesList(QList<DataStore::DataStoreVariable *> pVariables) const;

public slots:
    PyObject * values(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable) const;
};

//==============================================================================

class PythonWrapperNumPy : public QObject
{
    Q_OBJECT

public:
    explicit PythonWrapperNumPy(DataStore::DataStoreArray *pDataStoreArray, qulonglong pSize=0);
    ~PythonWrapperNumPy();

    PyObject * numpyArray() const;
    PyObject * pythonObject() const;

private:
    DataStore::DataStoreArray *mArray;
    PyObject *mNumPyArray;
    PyObject *mPythonObject;
};

//==============================================================================

}   // namespace PythonWrapper
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

