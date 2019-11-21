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

#pragma once

//==============================================================================

#include <QObject>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreArray
{
public:
    explicit DataStoreArray(quint64 pSize);

    quint64 size() const;

    double * data() const;
    double data(quint64 pPosition) const;

    void reset();

    void hold();
    void release();

private:
    int mReferenceCounter = 1;

    quint64 mSize;
    double *mData = nullptr;
};

//==============================================================================

class DataStoreValue : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreValue(double *pValue = nullptr);

    void setUri(const QString &pUri);

public slots:
    QString uri() const;

    double value() const;
    void setValue(double pValue);

private:
    QString mUri;

    double *mValue;
};

//==============================================================================

class DataStoreValues : public QList<DataStoreValue *>
{
public:
    explicit DataStoreValues(DataStoreArray *pDataStoreArray);
    ~DataStoreValues();
};

//==============================================================================

class DataStoreVariableRun : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariableRun(quint64 pCapacity, double *pValue);
    ~DataStoreVariableRun() override;

    quint64 size() const;

    DataStoreArray * array() const;

    void addValue();
    void addValue(double pValue);

    double value(quint64 pPosition) const;
    double * values() const;

private:
    quint64 mCapacity;
    quint64 mSize = 0;

    DataStoreArray *mArray = nullptr;
    double *mValue;
};

//==============================================================================

using DataStoreVariableRuns = QList<DataStoreVariableRun *>;

//==============================================================================

class DataStoreVariable : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariable(double *pValue = nullptr);
    ~DataStoreVariable() override;

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    bool addRun(quint64 pCapacity);
    void keepRuns(int pRunsCount);

    int type() const;
    void setType(int pType);

    void setUri(const QString &pUri);
    void setLabel(const QString &pLabel);
    void setUnit(const QString &pUnit);

    DataStoreArray * array(int pRun = -1) const;

    void addValue();
    void addValue(double pValue, int pRun = -1);

    double * values(int pRun = -1) const;

public slots:
    bool isVisible() const;

    int runsCount() const;

    QString uri() const;
    QString label() const;
    QString unit() const;

    quint64 size(int pRun = -1) const;

    double value(quint64 pPosition, int pRun = -1) const;

    double value() const;
    void setValue(double pValue);

private:
    int mType = -1;
    QString mUri;
    QString mName;
    QString mUnit;

    double *mValue;

    DataStoreVariableRuns mRuns;
};

//==============================================================================

using DataStoreVariables = QList<DataStoreVariable *>;

//==============================================================================

class DataStore;

//==============================================================================

class DataStoreData : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreData(const QString &pFileName);

    QString fileName() const;

protected:
    QString mFileName;
};

//==============================================================================

class DataStoreImportData : public DataStoreData
{
    Q_OBJECT

public:
    explicit DataStoreImportData(const QString &pFileName,
                                 DataStore *pImportDataStore,
                                 DataStore *pResultsDataStore,
                                 int pNbOfVariables,
                                 quint64 pNbOfDataPoints,
                                 const QList<quint64> &pRunSizes);
    ~DataStoreImportData() override;

    bool valid() const;

    DataStore * importDataStore() const;
    DataStore * resultsDataStore() const;

    QStringList hierarchy() const;

    int nbOfVariables() const;
    quint64 nbOfDataPoints() const;

    double * importValues() const;
    double * resultsValues() const;

    DataStoreVariables importVariables() const;
    DataStoreVariables resultsVariables() const;

    QList<quint64> runSizes() const;

    double progress();

private:
    bool mValid = true;

    DataStore *mImportDataStore;
    DataStore *mResultsDataStore;

    QStringList mHierarchy;

    int mNbOfVariables;
    quint64 mNbOfDataPoints;

    double *mImportValues;
    double *mResultsValues;

    DataStoreVariables mImportVariables;
    DataStoreVariables mResultsVariables;

    QList<quint64> mRunSizes;

    quint64 mProgress = 0;
    double mOneOverTotalProgress;
};

//==============================================================================

class DataStoreExportData : public DataStoreData
{
    Q_OBJECT

public:
    explicit DataStoreExportData(const QString &pFileName,
                                 DataStore *pDataStore,
                                 const DataStoreVariables &pVariables);

    DataStore * dataStore() const;

    DataStoreVariables variables() const;

private:
    DataStore *mDataStore = nullptr;

    DataStoreVariables mVariables;
};

//==============================================================================

class DataStore : public QObject
{
    Q_OBJECT

public:
    explicit DataStore(const QString &pUri = QString());
    ~DataStore() override;

    bool addRun(quint64 pCapacity);

    DataStoreVariables variables();
    DataStoreVariables voiAndVariables();

    DataStoreVariables addVariables(double *pValues, int pCount);
    DataStoreVariable * addVariable(double *pValue);

    void removeVariables(const DataStoreVariables &pVariables);
    void removeVariable(DataStoreVariable *pVariable);

    void addValues(double pVoiValue);

public slots:
    QString uri() const;

    int runsCount() const;

    quint64 size(int pRun = -1) const;

    OpenCOR::DataStore::DataStoreVariable * voi() const;

private:
    QString mUri;

    DataStoreVariable *mVoi = nullptr;
    DataStoreVariables mVariables;
};

//==============================================================================

class DataStoreImporterWorker : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreImporterWorker(DataStoreImportData *pImportData);

protected:
    DataStoreImportData *mImportData = nullptr;

signals:
    void progress(DataStoreImportData *pImportData, double pProgress);
    void done(DataStoreImportData *pImportData, const QString &pErrorMessage);

public slots:
    virtual void run() = 0;
};

//==============================================================================

class DataStoreImporter : public QObject
{
    Q_OBJECT

public:
    void importData(DataStoreImportData *pImportData);

protected:
    virtual DataStoreImporterWorker * workerInstance(DataStoreImportData *pImportData) = 0;

signals:
    void progress(DataStoreImportData *pImportData, double pProgress);
    void done(DataStoreImportData *pImportData, const QString &pErrorMessage);
};

//==============================================================================

class DataStoreExporterWorker : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreExporterWorker(DataStoreExportData *pDataStoreData);

protected:
    DataStoreExportData *mDataStoreData = nullptr;

signals:
    void progress(DataStoreExportData *pDataStoreData, double pProgress);
    void done(DataStoreExportData *pDataStoreData, const QString &pErrorMessage);

public slots:
    virtual void run() = 0;
};

//==============================================================================

class DataStoreExporter : public QObject
{
    Q_OBJECT

public:
    void exportData(DataStoreExportData *pDataStoreData);

protected:
    virtual DataStoreExporterWorker * workerInstance(DataStoreExportData *pDataStoreData) = 0;

signals:
    void progress(DataStoreExportData *pDataStoreData, double pProgress);
    void done(DataStoreExportData *pDataStoreData, const QString &pErrorMessage);
};

//==============================================================================

} // namespace DataStore

//==============================================================================

extern "C" Q_DECL_EXPORT int dataStoreInterfaceVersion();

//==============================================================================

class DataStoreInterface
{
public:
    virtual ~DataStoreInterface();

#define INTERFACE_DEFINITION
    #include "datastoreinterface.inl"
#undef INTERFACE_DEFINITION
};

//==============================================================================

using DataStoreInterfaces = QList<DataStoreInterface *>;

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::DataStoreInterface, "OpenCOR::DataStoreInterface")

//==============================================================================
// End of file
//==============================================================================
