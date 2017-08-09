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

#pragma once

//==============================================================================

#include <PythonQt/PythonQtPythonInclude.h>

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace DataStore {

//==============================================================================

class DataStore;
class DataStoreArray;
class DataStoreVariable;
class DataStoreVariables;

//==============================================================================

class DataStorePythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit DataStorePythonWrapper(PyObject *pModule, QObject *pParent=0);

    static PyTypeObject valuesdict_type;

    static PyObject * newNumPyArray(DataStoreArray *pDataStoreArray);
    static PyObject * newNumPyArray(DataStoreVariable *pDataStoreVariable);

    static PyObject *dataStoreValuesDict(const DataStoreVariables &pDataStoreVariables);
    static PyObject *dataStoreVariablesDict(const DataStoreVariables &pDataStoreVariables);

public slots:
    PyObject * values(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable) const;

    PyObject * variables(OpenCOR::DataStore::DataStore *pDataStore);
    PyObject * voiAndVariables(OpenCOR::DataStore::DataStore *pDataStore);
};

//==============================================================================

class NumPyPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit NumPyPythonWrapper(DataStoreArray *pDataStoreArray, qulonglong pSize=0);
    ~NumPyPythonWrapper();

    PyObject * numpyArray() const;
    PyObject * pythonObject() const;

private:
    DataStoreArray *mArray;
    PyObject *mNumPyArray;
    PyObject *mPythonObject;
};

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================

