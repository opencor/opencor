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

DataStoreVariable::DataStoreVariable(const qulonglong &pCapacity,
                                     double *pValue) :
#ifndef CLI_VERSION
    mIcon(QIcon()),
#endif
    mUri(QString()),
    mName(QString()),
    mUnit(QString()),
    mCapacity(pCapacity),
    mSize(0),
    mValue(pValue)
{
    // Create our array of values

    mValues = new double[pCapacity];
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    // Delete some internal objects

    delete[] mValues;
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
    //       those that the user can see in the GUI (e.g. in the Simulation
    //       Experiment view). See issues #568 and #808 for some background on
    //       this...

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
    Q_ASSERT(mValue);

    mValues[mSize] = *mValue;

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

DataStore::DataStore(const QString &pUri,
                     const qulonglong &pCapacity) :
    mlUri(pUri),
    mCapacity(pCapacity),
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

DataStoreVariable * DataStore::addVariable(double *pValue)
{
    // Add a variable to our data store

    DataStoreVariable *variable = new DataStoreVariable(mCapacity, pValue);

    mVariables << variable;

    return variable;
}

//==============================================================================

DataStoreVariables DataStore::addVariables(const int &pCount,
                                           double *pValues)
{
    // Add some variables to our data store

    DataStoreVariables variables = DataStoreVariables();

    for (int i = 0; i < pCount; ++i, ++pValues)
        variables << new DataStoreVariable(mCapacity, pValues);

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
