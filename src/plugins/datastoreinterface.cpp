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
#include "solverinterface.h"

//==============================================================================

#include <QThread>

//==============================================================================

namespace OpenCOR {

//==============================================================================

extern "C" Q_DECL_EXPORT int dataStoreInterfaceVersion()
{
    // Version of the data store interface

    return 5;
}

//==============================================================================

namespace DataStore {

//==============================================================================

DataStoreArray::DataStoreArray(quint64 pSize) :
    mSize(pSize)
{
    mData = new double[pSize]{};
    mRefCount = 1;
}

//==============================================================================

DataStoreArray::~DataStoreArray()
{
}

//==============================================================================

quint64 DataStoreArray::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

double * DataStoreArray::data() const
{
    // Return a pointer to our data

    return mData;
}

//==============================================================================

double DataStoreArray::data(quint64 pPosition) const
{
    // Return the value at the given position

    Q_ASSERT((pPosition < mSize) && mData);

    return mData[pPosition];
}

//==============================================================================

void DataStoreArray::clear()
{
    // Clear our data

    memset(mData, 0, mSize*Solver::SizeOfDouble);
}

//==============================================================================

void DataStoreArray::decRef()
{
    --mRefCount;
    if (mRefCount == 0) {
        delete[] mData;
        delete this;
    }
}

//==============================================================================

void DataStoreArray::incRef()
{
    ++mRefCount;
}

//==============================================================================

DataStoreValue::DataStoreValue(double *pValue) :
    mUri(QString()),
    mValue(pValue)
{
}

//==============================================================================

DataStoreValue::~DataStoreValue()
{
}

//==============================================================================

void DataStoreValue::setUri(const QString &pUri)
{
    // Set our URI

    mUri = pUri;
}

//==============================================================================

QString DataStoreValue::uri() const
{
    // Return our URI

    return mUri;
}

//==============================================================================

double DataStoreValue::value() const
{
    // Return our value

    Q_ASSERT(mValue);

    return *mValue;
}

//==============================================================================

void DataStoreValue::setValue(double pValue)
{
    // Set our value

    Q_ASSERT(mValue);

    *mValue = pValue;
}

//==============================================================================

DataStoreValues::DataStoreValues()
{
}

//==============================================================================

DataStoreValues::DataStoreValues(DataStoreArray *pDataStoreArray)
{
    // Create a list of DataStoreValues

    double *d = pDataStoreArray->data();
    quint64 i = pDataStoreArray->size();

    while (i-- > 0) {
        auto v = new DataStoreValue(d);
        *this << v;
        ++d;
    }
}

//==============================================================================

DataStoreValues::~DataStoreValues()
{
    // Delete our DataStoreValues

    while (count() > 0) {
        delete last();

        removeLast();
    }
}

//==============================================================================

DataStoreVariableRun::DataStoreVariableRun(quint64 pCapacity, double *pValue) :
    mCapacity(pCapacity),
    mSize(0),
    mValue(pValue)
{
    // Create our array of values

    mArray = new DataStoreArray(mCapacity);
    mValues = mArray->data();
}

//==============================================================================

DataStoreVariableRun::~DataStoreVariableRun()
{
    // Delete some internal objects

    mArray->decRef();
}

//==============================================================================

quint64 DataStoreVariableRun::size() const
{
    // Return our size

    return mSize;
}

//==============================================================================

void DataStoreVariableRun::addValue()
{
    // Set the value of the variable at the given position

    Q_ASSERT((mSize < mCapacity) && mValue);

    mValues[mSize] = *mValue;

    ++mSize;
}

//==============================================================================

void DataStoreVariableRun::addValue(double pValue)
{
    // Set the value of the variable at the given position using the given value

    Q_ASSERT(mSize < mCapacity);

    mValues[mSize] = pValue;

    ++mSize;
}

//==============================================================================

DataStoreArray * DataStoreVariableRun::array() const
{
    // Return our data array

    return mArray;
}

//==============================================================================

double DataStoreVariableRun::value(quint64 pPosition) const
{
    // Return the value at the given position

    Q_ASSERT(pPosition < mSize);

    return mValues[pPosition];
}

//==============================================================================

double * DataStoreVariableRun::values() const
{
    // Return our values

    return mValues;
}

//==============================================================================

DataStoreVariable::DataStoreVariable(double *pValue) :
    mType(-1),
    mUri(QString()),
    mName(QString()),
    mUnit(QString()),
    mValue(pValue),
    mRuns(DataStoreVariableRuns())
{
}

//==============================================================================

DataStoreVariable::~DataStoreVariable()
{
    // Delete some internal objects

    for (auto run : mRuns) {
        delete run;
    }
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

int DataStoreVariable::runsCount() const
{
    // Return our number of runs

    return mRuns.count();
}

//==============================================================================

bool DataStoreVariable::addRun(quint64 pCapacity)
{
    // Try to add a run of the given capacity

    try {
        mRuns << new DataStoreVariableRun(pCapacity, mValue);
    } catch (...) {
        return false;
    }

    return true;
}

//==============================================================================

void DataStoreVariable::keepRuns(int pRunsCount)
{
    // Keep the given number of runs

    while (mRuns.count() > pRunsCount) {
        delete mRuns.last();

        mRuns.removeLast();
    }
}

//==============================================================================

int DataStoreVariable::type() const
{
    // Return our type

    return mType;
}

//==============================================================================

void DataStoreVariable::setType(int pType)
{
    // Set our type

    mType = pType;
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

quint64 DataStoreVariable::size(int pRun) const
{
    // Return our size for the given run

    if (pRun == -1) {
        return (!mRuns.isEmpty())?
                   mRuns.last()->size():
                   0;
    }

    return ((pRun >= 0) && (pRun < mRuns.count()))?
               mRuns[pRun]->size():
               0;
}

//==============================================================================

void DataStoreVariable::addValue()
{
    // Add a value to our current (i.e. last) run

    Q_ASSERT(!mRuns.isEmpty());

    mRuns.last()->addValue();
}

//==============================================================================

void DataStoreVariable::addValue(double pValue, int pRun)
{
    // Add the given value to our current (i.e. last) run

    Q_ASSERT(!mRuns.isEmpty());

    if (pRun == -1) {
        mRuns.last()->addValue(pValue);
    } else {
        Q_ASSERT((pRun >= 0) && (pRun < mRuns.count()));

        mRuns[pRun]->addValue(pValue);
    }
}

//==============================================================================

double DataStoreVariable::value(quint64 pPosition, int pRun) const
{
    // Return the value at the given position and this for the given run

    Q_ASSERT(!mRuns.isEmpty());

    if (pRun == -1) {
        return mRuns.last()->value(pPosition);
    }

    Q_ASSERT((pRun >= 0) && (pRun < mRuns.count()));

    return mRuns[pRun]->value(pPosition);
}

//==============================================================================

double * DataStoreVariable::values(int pRun) const
{
    // Return the values for the given run, if any

    if (mRuns.isEmpty()) {
        return nullptr;
    }

    if (pRun == -1) {
        return mRuns.last()->values();
    }

    return ((pRun >= 0) && (pRun < mRuns.count()))?
               mRuns[pRun]->values():
               nullptr;
}

//==============================================================================

double DataStoreVariable::getValue() const
{
    // Return the value to be added next

    Q_ASSERT(mValue);

    return *mValue;
}

//==============================================================================

void DataStoreVariable::setValue(double pValue)
{
    // Set the value to be added next

    Q_ASSERT(mValue);

    *mValue = pValue;
}

//==============================================================================

DataStoreArray * DataStoreVariable::array(int pRun) const
{
    // Return the data array for the given run, if any

    if (mRuns.isEmpty()) {
        return 0;
    } else {
        if (pRun == -1)
            return mRuns.last()->array();
        else
            return ((pRun >= 0) && (pRun < mRuns.count()))?mRuns[pRun]->array():0;
    }
}

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

DataStoreImportData::DataStoreImportData(const QString &pFileName,
                                         DataStore *pImportDataStore,
                                         DataStore *pResultsDataStore,
                                         int pNbOfVariables,
                                         quint64 pNbOfDataPoints,
                                         const QList<quint64> &pRunSizes) :
    DataStoreData(pFileName),
    mValid(true),
    mImportDataStore(pImportDataStore),
    mResultsDataStore(pResultsDataStore),
    mNbOfVariables(pNbOfVariables),
    mNbOfDataPoints(pNbOfDataPoints),
    mImportVariables(DataStoreVariables()),
    mResultsVariables(DataStoreVariables()),
    mRunSizes(pRunSizes),
    mProgress(0),
    mOneOverTotalProgress(1.0/pNbOfDataPoints)
{
    // Initialise our hierarchy

    static int importNb = 0;

    mHierarchy = QStringList() << "imports" << QString("import_%1").arg(++importNb);

    // Allocate space for our import/results values, as well as add the required
    // number of variables for our import/results data store and a run that will
    // contain all of our raw/computed imported data in our import/results data
    // store
    // Note: we make several calls to DataStore::addVariable() rather than one
    //       big one to DataStore::addVariables() in case we can't allocate
    //       enough memory, in which case we will need to remove the variables
    //       we have added, and a failing call to DataStore::addVariables()
    //       wouldn't allow us to do this since that call wouldn't actually
    //       return...

    try {
        mImportValues = new double[pNbOfVariables] {};
        mResultsValues = new double[pNbOfVariables] {};

        for (int i = 0; i < pNbOfVariables; ++i) {
            mImportVariables << pImportDataStore->addVariable(mImportValues+i);
            mResultsVariables << pResultsDataStore->addVariable(mResultsValues+i);
        }

        if (!pImportDataStore->addRun(pNbOfDataPoints)) {
            throw std::exception();
        }

        if (!pRunSizes.isEmpty()) {
            for (auto runSize : pRunSizes) {
                for (auto resultsVariables : mResultsVariables) {
                    if (!resultsVariables->addRun(runSize)) {
                        throw std::exception();
                    }
                }
            }
        }
    } catch (...) {
        // Something went wrong, so release the memory that was directly or
        // indirectly allocated

        mValid = false;

        delete mImportValues;
        delete mResultsValues;

        mImportValues = nullptr;
        mResultsValues = nullptr;

        pImportDataStore->removeVariables(mImportVariables);
        pResultsDataStore->removeVariables(mResultsVariables);
    }
}

//==============================================================================

DataStoreImportData::~DataStoreImportData()
{
    // Delete some internal objects
    // Note: we must not delete mResultsValues since it is to be deleted by
    //       whoever used us (e.g. a Simulation object)...

    delete mImportValues;
}

//==============================================================================

bool DataStoreImportData::valid() const
{
    // Return whether we are valid

    return mValid;
}

//==============================================================================

DataStore * DataStoreImportData::importDataStore() const
{
    // Return our import data store

    return mImportDataStore;
}

//==============================================================================

DataStore * DataStoreImportData::resultsDataStore() const
{
    // Return our results data store

    return mResultsDataStore;
}

//==============================================================================

QStringList DataStoreImportData::hierarchy() const
{
    // Return our hierarchy

    return mHierarchy;
}

//==============================================================================

int DataStoreImportData::nbOfVariables() const
{
    // Return our number of variables

    return mNbOfVariables;
}

//==============================================================================

quint64 DataStoreImportData::nbOfDataPoints() const
{
    // Return our number of data points

    return mNbOfDataPoints;
}

//==============================================================================

double * DataStoreImportData::importValues() const
{
    // Return our import values

    return mImportValues;
}

//==============================================================================

double * DataStoreImportData::resultsValues() const
{
    // Return our results values

    return mResultsValues;
}

//==============================================================================

DataStoreVariables DataStoreImportData::importVariables() const
{
    // Return our import variables

    return mImportVariables;
}

//==============================================================================

DataStoreVariables DataStoreImportData::resultsVariables() const
{
    // Return our results variables

    return mResultsVariables;
}

//==============================================================================

QList<quint64> DataStoreImportData::runSizes() const
{
    // Return our run sizes

    return mRunSizes;
}

//==============================================================================

double DataStoreImportData::progress()
{
    // Increase and return our normalised progress

    return (++mProgress)*mOneOverTotalProgress;
}

//==============================================================================

DataStoreExportData::DataStoreExportData(const QString &pFileName, DataStore *pDataStore,
                                         const DataStoreVariables &pVariables) :
    DataStoreData(pFileName),
    mDataStore(pDataStore),
    mVariables(pVariables)
{
}

//==============================================================================

DataStore * DataStoreExportData::dataStore() const
{
    // Return our data store

    return mDataStore;
}

//==============================================================================

DataStoreVariables DataStoreExportData::variables() const
{
    // Return our variables

    return mVariables;
}

//==============================================================================

DataStore::DataStore(const QString &pUri) :
    mUri(pUri),
    mVoi(new DataStoreVariable()),
    mVariables(DataStoreVariables())
{
}

//==============================================================================

DataStore::~DataStore()
{
    // Delete some internal objects

    delete mVoi;

    for (auto variable : mVariables) {
        delete variable;
    }
}

//==============================================================================

QString DataStore::uri() const
{
    // Return our URI

    return mUri;
}

//==============================================================================

int DataStore::runsCount() const
{
    // Return our number of runs, i.e. the number of runs for our VOI, for
    // example

    return mVoi->runsCount();
}

//==============================================================================

bool DataStore::addRun(quint64 pCapacity)
{
    // Try to add a run to our VOI and all our variables

    int oldRunsCount = mVoi->runsCount();

    try {
        if (!mVoi->addRun(pCapacity)) {
            throw std::exception();
        }

        for (auto variable : mVariables) {
            if (!variable->addRun(pCapacity)) {
                throw std::exception();
            }
        }
    } catch (...) {
        // We couldn't add a run to our VOI and all our variables, so only keep
        // the number of runs we used to have

        mVoi->keepRuns(oldRunsCount);

        for (auto variable : mVariables) {
            variable->keepRuns(oldRunsCount);
        }

        return false;
    }

    return true;
}

//==============================================================================

quint64 DataStore::size(int pRun) const
{
    // Return our size, i.e. the size of our VOI, for example

    return mVoi->size(pRun);
}

//==============================================================================

DataStoreVariable * DataStore::voi() const
{
    // Return our VOI

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

DataStoreVariables DataStore::voiAndVariables()
{
    // Return our VOI, if any, and all our variables, after making sure that
    // they are sorted

    DataStoreVariables res = DataStoreVariables();

    res << mVoi << mVariables;

    std::sort(res.begin(), res.end(), DataStoreVariable::compare);

    return res;
}

//==============================================================================

DataStoreVariables DataStore::addVariables(double *pValues, int pCount)
{
    // Add some variables to our data store

    DataStoreVariables variables = DataStoreVariables();

    for (int i = 0; i < pCount; ++i, ++pValues) {
        variables << new DataStoreVariable(pValues);
    }

    mVariables << variables;

    return variables;
}

//==============================================================================

DataStoreVariable * DataStore::addVariable(double *pValue)
{
    // Add a variable to our data store

    auto variable = new DataStoreVariable(pValue);

    mVariables << variable;

    return variable;
}

//==============================================================================

void DataStore::removeVariables(const DataStoreVariables &pVariables)
{
    // Remove the given variables from our data store

    for (auto variable : pVariables) {
        delete variable;

        mVariables.removeOne(variable);
    }
}

//==============================================================================

void DataStore::removeVariable(DataStoreVariable *pVariable)
{
    // Remove the given variable from our data store

    delete pVariable;

    mVariables.removeOne(pVariable);
}

//==============================================================================

void DataStore::addValues(double pVoiValue)
{
    // Set the value at the mSize position of all our variables including our
    // VOI, which value is directly given to us
    // Note: it is very important to add the VOI value last since our size()
    //       method relies on it to determine our size. So, if we were to add
    //       the VOI value first, we might in some cases (see issue #1579 for
    //       example) end up with the wrong size...

    for (auto variable : mVariables) {
        variable->addValue();
    }

    mVoi->addValue(pVoiValue);
}

//==============================================================================

DataStoreImporterWorker::DataStoreImporterWorker(DataStoreImportData *pImportData) :
    mImportData(pImportData)
{
}

//==============================================================================

void DataStoreImporter::importData(DataStoreImportData *pImportData)
{
    // Create and move our worker to a thread
    // Note: we cannot use the new connect() syntax with our worker's signals
    //       since it's an instance of a derived class located in another
    //       plugin...

    auto thread = new QThread();
    DataStoreImporterWorker *worker = workerInstance(pImportData);

    worker->moveToThread(thread);

    connect(thread, &QThread::started,
            worker, &DataStoreImporterWorker::run);

    connect(worker, SIGNAL(progress(DataStoreImportData *, double)),
            this, SIGNAL(progress(DataStoreImportData *, double)));

    connect(worker, SIGNAL(done(DataStoreImportData *, const QString &)),
            this, SIGNAL(done(DataStoreImportData *, const QString &)));
    connect(worker, SIGNAL(done(DataStoreImportData *, const QString &)),
            thread, SLOT(quit()));
    connect(worker, SIGNAL(done(DataStoreImportData *, const QString &)),
            worker, SLOT(deleteLater()));

    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);

    // Start our worker by starting the thread in which it is

    thread->start();
}

//==============================================================================

DataStoreExporterWorker::DataStoreExporterWorker(DataStoreExportData *pDataStoreData) :
    mDataStoreData(pDataStoreData)
{
}

//==============================================================================

void DataStoreExporter::exportData(DataStoreExportData *pDataStoreData)
{
    // Create and move our worker to a thread
    // Note: we cannot use the new connect() syntax with our worker's signals
    //       since it's an instance of a derived class located in another
    //       plugin...

    auto thread = new QThread();
    DataStoreExporterWorker *worker = workerInstance(pDataStoreData);

    worker->moveToThread(thread);

    connect(thread, &QThread::started,
            worker, &DataStoreExporterWorker::run);

    connect(worker, SIGNAL(progress(DataStoreExportData *, double)),
            this, SIGNAL(progress(DataStoreExportData *, double)));

    connect(worker, SIGNAL(done(DataStoreExportData *, const QString &)),
            this, SIGNAL(done(DataStoreExportData *, const QString &)));
    connect(worker, SIGNAL(done(DataStoreExportData *, const QString &)),
            thread, SLOT(quit()));
    connect(worker, SIGNAL(done(DataStoreExportData *, const QString &)),
            worker, SLOT(deleteLater()));

    connect(thread, &QThread::finished,
            thread, &QThread::deleteLater);

    // Start our worker by starting the thread in which it is

    thread->start();
}

//==============================================================================

} // namespace DataStore

//==============================================================================

DataStoreInterface::~DataStoreInterface() = default;

//==============================================================================

} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
