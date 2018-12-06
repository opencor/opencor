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
#include "pythonqtinclude.h"

//==============================================================================

#include <QObject>

//==============================================================================

#include <functional>

//==============================================================================

namespace OpenCOR {

//==============================================================================

namespace SimulationSupport {
    class SimulationData;

#if defined(_MSC_VER)
    typedef std::_Binder<std::_Unforced, void (*)(SimulationData *), SimulationData * const> SimulationDataUpdatedFunction;
#elif defined(__APPLE__)
    typedef std::__bind<void (*)(SimulationData *), SimulationData *> SimulationDataUpdatedFunction;
#else
    typedef std::_Bind_helper<false, void (*)(SimulationData *), SimulationData *>::type SimulationDataUpdatedFunction;
#endif
}   // namespace SimulationSupport

//==============================================================================

namespace DataStore {

//==============================================================================

typedef struct {
    PyDictObject dict;
    SimulationSupport::SimulationDataUpdatedFunction *mSimulationDataUpdatedFunction;
} DataStoreValuesDictObject;

//==============================================================================

class DataStore;
class DataStoreArray;
class DataStoreValues;
class DataStoreVariable;
class DataStoreVariables;

//==============================================================================

class DataStorePythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit DataStorePythonWrapper(PyObject *pModule, QObject *pParent=0);

    static PyTypeObject DataStoreValuesDict_Type;

    static DATASTORE_EXPORT PyObject * newNumPyArray(DataStoreArray *pDataStoreArray);
    static DATASTORE_EXPORT PyObject * newNumPyArray(DataStoreVariable *pDataStoreVariable, const int &pRun);

    static DATASTORE_EXPORT PyObject *dataStoreValuesDict(const DataStoreValues *pDataStoreValues,
                                                          SimulationSupport::SimulationDataUpdatedFunction *pSimulationDataUpdatedFunction=NULL);
    static DATASTORE_EXPORT PyObject *dataStoreVariablesDict(const DataStoreVariables &pDataStoreVariables);

public slots:
    double valueAt(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable, const quint64 &pPosition, const int &pRun = -1) const;

    PyObject * values(OpenCOR::DataStore::DataStoreVariable *pDataStoreVariable, const int &pRun = -1) const;

    PyObject * variables(OpenCOR::DataStore::DataStore *pDataStore);
    PyObject * voiAndVariables(OpenCOR::DataStore::DataStore *pDataStore);
};

//==============================================================================

class NumpyPythonWrapper : public QObject
{
    Q_OBJECT

public:
    explicit NumpyPythonWrapper(DataStoreArray *pDataStoreArray, quint64 pSize=0);
    ~NumpyPythonWrapper();

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
