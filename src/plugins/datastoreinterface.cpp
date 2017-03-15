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

#ifdef OpenCOR_MAIN
    #include "cliutils.h"
#else
    #include "corecliutils.h"
#endif
#include "datastoreinterface.h"

//==============================================================================

#include <QThread>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int dataStoreInterfaceVersion()
{
    // Version of the data store interface

    return 1;
}

//==============================================================================

namespace DataStore {

//==============================================================================

DataStoreArray::DataStoreArray(const qulonglong &pCapacity) :
    mCapacity(pCapacity),
    mReferences(0)
{
    mValues = new double[pCapacity];
    mReferences = 1;
}

//==============================================================================

DataStoreArray::~DataStoreArray()
{
    decReference();
}

//==============================================================================

void DataStoreArray::decReference()
{
    --mReferences;
    if (mReferences == 0) {
        delete[] mValues;
        mValues = 0;
    }
}

//==============================================================================

void DataStoreArray::incReference()
{
    ++mReferences;
}

//==============================================================================

qulonglong DataStoreArray::capacity() const
{
    // Return our capacity

    return mCapacity;
}

//==============================================================================

double * DataStoreArray::values() const
{
    // Return our values

    return mValues;
}

//==============================================================================

DataStoreVariable::DataStoreVariable(const qulonglong &pCapacity, double *pNextValuePtr) :
#ifndef CLI_VERSION
    mIcon(QIcon()),
#endif
    mUri(QString()),
    mName(QString()),
    mUnit(QString()),
    mCapacity(0),
    mSize(0),
    mArray(0),
    mNextValuePtr(pNextValuePtr),
    mValues(0)
{
    if (pCapacity) createArray(pCapacity);
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    if (mCapacity) deleteArray();
}

//==============================================================================

void DataStoreVariable::createArray(const qulonglong &pCapacity)
{
    mCapacity = pCapacity;
    mSize = 0;

    // Create our array of values

    mArray = new DataStoreArray(mCapacity);
    mValues = mArray->values();
}

//==============================================================================

void DataStoreVariable::deleteArray()
{
    // Delete some internal objects

    if (mArray) mArray->decReference();

    mCapacity = 0;
    mSize = 0;
}

//==============================================================================

bool DataStoreVariable::compare(DataStoreVariable *pVariable1,
                                DataStoreVariable *pVariable2)
{
    // Determine which of the two variables should be first based on their URI
    // Note: the comparison is case insensitive, so that it's easier for people
    //       to find a variable...

    return pVariable1->uri().compare(pVariable2->uri(), Qt::CaseInsensitive) < 0;
}

//==============================================================================

bool DataStoreVariable::isVisible() const
{
    // Return whether we are visible, i.e. we have a non-empty URI
    // Note: this applies to CellML 1.1 file where we keep track of all the
    //       model parameters (including imported ones), but should only export
    //       those that the user can see in the GUI (e.g. in the Single Cell
    //       view). See issues #568 and #808 for some background on this...

    return !mUri.isEmpty();
}

//==============================================================================

#ifndef CLI_VERSION
QIcon DataStoreVariable::icon() const
{
    // Return our icon

    return mIcon;
}
#endif

//==============================================================================

#ifndef CLI_VERSION
void DataStoreVariable::setIcon(const QIcon &pIcon)
{
    // Set our icon

    mIcon = pIcon;
}
#endif

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

void DataStoreVariable::addValue()
{
    // Set the value of the variable at the given position

    Q_ASSERT(mSize < mCapacity);
    Q_ASSERT(mNextValuePtr);

    mValues[mSize] = *mNextValuePtr;

    ++mSize;
}

//==============================================================================

void DataStoreVariable::addValue(const double &pValue)
{
    // Set the value of the variable at the given position using the given value

    Q_ASSERT(mSize < mCapacity);

    mValues[mSize] = pValue;

    ++mSize;
}

//==============================================================================

DataStoreArray * DataStoreVariable::array() const
{
    // Return our data array

    return mArray;
}

//==============================================================================

void DataStoreVariable::clearNextValuePtr()
{
    mNextValuePtr = 0;
}

//==============================================================================

void DataStoreVariable::setNextValuePtr(double *pNextValuePtr)
{
    mNextValuePtr = pNextValuePtr;
}

//==============================================================================

double DataStoreVariable::nextValue() const
{
    Q_ASSERT(mNextValuePtr);
    return *mNextValuePtr;
}

//==============================================================================

void DataStoreVariable::setNextValue(const double &pValue)
{
    Q_ASSERT(mNextValuePtr);

    *mNextValuePtr = pValue;
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

void DataStoreVariables::clearNextValuePtrs()
{
    foreach (DataStoreVariable *variable, *this)
        variable->clearNextValuePtr();
}

//==============================================================================

void DataStoreVariables::setNextValuePtrs(double *pNextValuePtrs)
{
    foreach (DataStoreVariable *variable, *this) {
        variable->setNextValuePtr(pNextValuePtrs);
        ++pNextValuePtrs;
    }
}

//==============================================================================

DataStoreData::DataStoreData(const QString &pFileName,
                             const DataStoreVariables &pSelectedVariables) :
    mFileName(pFileName),
    mSelectedVariables(pSelectedVariables)
{
}

//==============================================================================

QString DataStoreData::fileName() const
{
    // Return our file name

    return mFileName;
}

//==============================================================================

DataStoreVariables DataStoreData::selectedVariables() const
{
    // Return our selected variables

    return mSelectedVariables;
}

//==============================================================================

DataStore::DataStore(const QString &pUri) :
    mlUri(pUri),
    mCapacity(0),
    mSize(0),
    mVoi(0),
    mVariables(DataStoreVariables())
{
}

//==============================================================================

DataStore::~DataStore()
{
    // Delete some internal objects

    delete mVoi;

#ifdef OpenCOR_MAIN
    resetList(mVariables);
#else
    Core::resetList(mVariables);
#endif
}

//==============================================================================

void DataStore::createArrays(const qulonglong &pCapacity)
{
    mCapacity = pCapacity;
    mSize = 0;

    mVoi->createArray(pCapacity);

    foreach (DataStoreVariable *variable, mVariables)
        variable->createArray(pCapacity);
}

//==============================================================================

void DataStore::deleteArrays()
{
    if (mVoi) mVoi->deleteArray();

    foreach (DataStoreVariable *variable, mVariables)
        variable->deleteArray();

    mCapacity = 0;
    mSize = 0;
}

//==============================================================================

QString DataStore::uri() const
{
    // Return our URI

    return mlUri;
}

//==============================================================================

qulonglong DataStore::capacity() const
{
    // Return our capacity

    return mCapacity;
}

//==============================================================================

qulonglong DataStore::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

DataStoreVariables DataStore::voiAndVariables()
{
    // Return our variable of integration, if any, and all our variables, after
    // making sure that they are sorted

    DataStoreVariables res = DataStoreVariables();

    res << mVoi << mVariables;

    std::sort(res.begin(), res.end(), DataStoreVariable::compare);

    return res;
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

    mVoi = new DataStoreVariable(mCapacity);

    return mVoi;
}

//==============================================================================

DataStoreVariables DataStore::variables()
{
    // Return all our variables, after making sure that they are sorted

    std::sort(mVariables.begin(), mVariables.end(), DataStoreVariable::compare);

    return mVariables;
}

//==============================================================================

DataStoreVariable * DataStore::addVariable(double *pNextValuePtr)
{
    // Add a variable to our data store

    DataStoreVariable *variable = new DataStoreVariable(mCapacity, pNextValuePtr);

    mVariables << variable;

    return variable;
}

//==============================================================================

DataStoreVariables DataStore::addVariables(const int &pCount,
                                           double *pNextValuePtrs)
{
    // Add some variables to our data store

    DataStoreVariables variables = DataStoreVariables();

    for (int i = 0; i < pCount; ++i, ++pNextValuePtrs)
        variables << new DataStoreVariable(mCapacity, pNextValuePtrs);

    mVariables << variables;

    return variables;
}

//==============================================================================

void DataStore::addValues(const double &pVoiValue)
{
    // Set the value at the mSize position of all our variables including our
    // variable of integration, which value is directly given to us

    Q_ASSERT(mSize < mCapacity);

    if (mVoi)
        mVoi->addValue(pVoiValue);

    for (auto variable = mVariables.constBegin(), variableEnd = mVariables.constEnd();
         variable != variableEnd; ++variable) {
        (*variable)->addValue();
    }

    ++mSize;
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

    QString errorMessage = QString();

    execute(errorMessage);

    // Let people know that we are done with the export

    emit done(errorMessage);
}

//==============================================================================

}   // namespace DataStore
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
