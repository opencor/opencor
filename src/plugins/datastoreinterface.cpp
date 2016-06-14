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
// Data store interface
//==============================================================================

#include "datastoreinterface.h"

//==============================================================================

#include <QThread>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

DataStoreData::DataStoreData(const QString &pFileName) :
    mFileName(pFileName)
{
}

//==============================================================================

QString DataStoreData::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

DataStoreVariable::DataStoreVariable(const qulonglong &pSize, double *pValue) :
    mUri(QString()),
    mName(QString()),
    mUnit(QString()),
    mSize(pSize),
    mValue(pValue)
{
    // Create our array of values

    mValues = new double[pSize];
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    // Delete some internal objects

    delete[] mValues;
}

//==============================================================================

bool DataStoreVariable::isValid() const
{
    // Return whether we are valid, i.e. we have a non-empty URI
    // Note: this applies to CellML 1.1 file where we keep track of all the
    //       model parameters (including imported ones), but should only export
    //       those that the user can see in the GUI (e.g. in the Single Cell
    //       view). See issues #568 and #808 for some background on this...

    return !mUri.isEmpty();
}

//==============================================================================

QString DataStoreVariable::uri() const
{
    // Return our URI

    return mUri;
}

//==============================================================================

void DataStoreVariable::setUri(const QString &pUri)
{
    // Set our URI

    mUri = pUri;
}

//==============================================================================

QString DataStoreVariable::label() const
{
    // Return our label

    return mName;
}

//==============================================================================

void DataStoreVariable::setLabel(const QString &pLabel)
{
    // Set our label

    mName = pLabel;
}

//==============================================================================

QString DataStoreVariable::unit() const
{
    // Return our unit

    return mUnit;
}

//==============================================================================

void DataStoreVariable::setUnit(const QString &pUnit)
{
    // Set our unit

    mUnit = pUnit;
}

//==============================================================================

qulonglong DataStoreVariable::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

void DataStoreVariable::setValue(const qulonglong &pPosition)
{
    // Set the value of the variable at the given position

    Q_ASSERT(pPosition < mSize);
    Q_ASSERT(mValue);

    mValues[pPosition] = *mValue;
}

//==============================================================================

void DataStoreVariable::setValue(const qulonglong &pPosition,
                                 const double &pValue)
{
    // Set the value of the variable at the given position using the given value

    Q_ASSERT(pPosition < mSize);

    mValues[pPosition] = pValue;
}

//==============================================================================

double DataStoreVariable::value(const qulonglong &pPosition) const
{
    // Return our value at the given position

    Q_ASSERT(pPosition < mSize);

    return mValues[pPosition];
}

//==============================================================================

double * DataStoreVariable::values() const
{
    // Return our values

    return mValues;
}

//==============================================================================

DataStore::DataStore(const QString &pUri,
                     const qulonglong &pSize) :
    mlUri(pUri),
    mSize(pSize),
    mVoi(0),
    mVariables(0)
{
}

//==============================================================================

DataStore::~DataStore()
{
    // Delete some internal objects

    delete mVoi;

    for (auto variable = mVariables.constBegin(), variableEnd = mVariables.constEnd();
         variable != variableEnd; ++variable) {
        delete *variable;
    }
}

//==============================================================================

QString DataStore::uri() const
{
    // Return our URI

    return mlUri;
}

//==============================================================================

qulonglong DataStore::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

DataStoreVariable * DataStore::voi() const
{
    // Return our variable of integration

    return mVoi;
}

//==============================================================================

DataStoreVariable * DataStore::addVoi()
{
    // Add a (new) variable of integration to our data store

    delete mVoi;

    mVoi = new DataStoreVariable(mSize);

    return mVoi;
}

//==============================================================================

bool sortVariables(DataStoreVariable *pVariable1, DataStoreVariable *pVariable2)
{
    // Determine which of the two variables should be first based on their URI
    // Note: the comparison is case insensitive, so that it's easier for people
    //       to find a variable...

    return pVariable1->uri().compare(pVariable2->uri(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

DataStoreVariables DataStore::variables()
{
    // Return all our variables, after making sure that they are sorted

    std::sort(mVariables.begin(), mVariables.end(), sortVariables);

    return mVariables;
}

//==============================================================================

DataStoreVariable * DataStore::addVariable(double *pValue)
{
    // Add a variable to our data store

    DataStoreVariable *variable = new DataStoreVariable(mSize, pValue);

    mVariables << variable;

    return variable;
}

//==============================================================================

DataStoreVariables DataStore::addVariables(const int &pCount,
                                           double *pValues)
{
    // Add some variables to our data store

    DataStoreVariables variables(pCount);

    for (int i = 0; i < pCount; ++i, ++pValues) {
        variables[i] = new DataStoreVariable(mSize, pValues);

        mVariables << variables[i];
    }

    return variables;
}

//==============================================================================

void DataStore::setValues(const qulonglong &pPosition, const double &pValue)
{
    // Set the value at the given position of all our variables including our
    // variable of integration, which value is directly given to us

    if (mVoi)
        mVoi->setValue(pPosition, pValue);

    for (auto variable = mVariables.constBegin(), variableEnd = mVariables.constEnd();
         variable != variableEnd; ++variable) {
        (*variable)->setValue(pPosition);
    }
}

//==============================================================================

DataStoreExporter::DataStoreExporter(const QString &pFileName,
                                     DataStore *pDataStore,
                                     DataStoreData *pDataStoreData) :
    mFileName(pFileName),
    mDataStore(pDataStore),
    mDataStoreData(pDataStoreData)
{
    // Create our thread

    mThread = new QThread();

    // Move ourselves to our thread

    moveToThread(mThread);

    // Create a few connections

    connect(mThread, SIGNAL(started()),
            this, SLOT(started()));

    connect(mThread, SIGNAL(finished()),
            mThread, SLOT(deleteLater()));
    connect(mThread, SIGNAL(finished()),
            this, SLOT(deleteLater()));
}

//==============================================================================

DataStoreExporter::~DataStoreExporter()
{
    // Delete some internal objects
    // Note: no need to delete mDataStore since it will be automatically
    //       deleted through our threaded mechanism...

    delete mDataStoreData;
}

//==============================================================================

void DataStoreExporter::start()
{
    // Start the export

    mThread->start();
}

//==============================================================================

void DataStoreExporter::started()
{
    // Do the export itself

    execute();

    // Let people know that we are done with the export

    emit done();
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
