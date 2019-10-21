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

#include "datastoreglobal.h"
#include "datastoreinterface.h"
#include "simulationsupportpythonwrapper.h"

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStorePythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit DataStorePythonWrapper(PyObject *pModule, QObject *pParent);

    static PyTypeObject DataStoreValuesDict_Type;

    static DATASTORE_EXPORT PyObject * newNumPyArray(DataStoreArray *pDataStoreArray);
    static DATASTORE_EXPORT PyObject * newNumPyArray(DataStoreVariable *pDataStoreVariable,
                                                     int pRun = -1);

    static DATASTORE_EXPORT PyObject *dataStoreValuesDict(const DataStoreValues *pDataStoreValues,
                                                          SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction = nullptr);
    static DATASTORE_EXPORT PyObject *dataStoreVariablesDict(const DataStoreVariables &pDataStoreVariables);

public slots:
    double value(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable,
                 quint64 pPosition, int pRun = -1) const;
    PyObject * values(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable,
                      int pRun = -1) const;

    PyObject * variables(OpenCOR::DataStore::DataStore *pDataStore);
    PyObject * voiAndVariables(OpenCOR::DataStore::DataStore *pDataStore);
};

//==============================================================================

class NumPyPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit NumPyPythonWrapper(DataStoreArray *pDataStoreArray,
                                quint64 pSize = 0);
    ~NumPyPythonWrapper();

    PyObject * numPyArray() const;
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

