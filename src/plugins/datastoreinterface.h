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

#include "plugininfo.h"

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreVariableRun : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariableRun(quint64 pCapacity, double *pValue);
    ~DataStoreVariableRun();

    quint64 size() const;

    void addValue();
    void addValue(double pValue);

    double value(quint64 pPosition) const;
    double * values() const;

private:
    quint64 mCapacity;
    quint64 mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

typedef QList<DataStoreVariableRun *> DataStoreVariableRuns;

//==============================================================================

class DataStoreVariable : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariable(double *pValue = nullptr);
    ~DataStoreVariable();

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    bool isVisible() const;

    int runsCount() const;

    bool addRun(quint64 pCapacity);
    void keepRuns(int pRunsCount);

    int type() const;
    void setType(int pType);

    QString uri() const;
    void setUri(const QString &pUri);

    QString label() const;
    void setLabel(const QString &pLabel);

    QString unit() const;
    void setUnit(const QString &pUnit);

    quint64 size(int pRun = -1) const;

    void addValue();
    void addValue(double pValue, int pRun = -1);

    double value(quint64 pPosition, int pRun = -1) const;
    double * values(int pRun = -1) const;

private:
    int mType;
    QString mUri;
    QString mName;
    QString mUnit;

    double *mValue;

    DataStoreVariableRuns mRuns;
};

//==============================================================================

typedef QList<DataStoreVariable *> DataStoreVariables;

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
    ~DataStoreImportData();

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
    bool mValid;

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

    quint64 mProgress;
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
    DataStore *mDataStore;

    DataStoreVariables mVariables;
};

//==============================================================================

class DataStore : public QObject
{
    Q_OBJECT

public:
    explicit DataStore(const QString &pUri = QString());
    ~DataStore();

    QString uri() const;

    int runsCount() const;

    bool addRun(quint64 pCapacity);

    quint64 size(int pRun = -1) const;

    DataStoreVariable * voi() const;
    DataStoreVariables variables();
    DataStoreVariables voiAndVariables();

    DataStoreVariables addVariables(double *pValues, int pCount);
    DataStoreVariable * addVariable(double *pValue);

    void removeVariables(const DataStoreVariables &pVariables);
    void removeVariable(DataStoreVariable *pVariable);

    void addValues(double pVoiValue);

private:
    QString mUri;

    DataStoreVariable *mVoi;
    DataStoreVariables mVariables;
};

//==============================================================================

class DataStoreImporterWorker : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreImporterWorker(DataStoreImportData *pImportData);

protected:
    DataStoreImportData *mImportData;

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
    DataStoreExportData *mDataStoreData;

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

typedef QList<DataStoreInterface *> DataStoreInterfaces;

//==============================================================================

} // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::DataStoreInterface, "OpenCOR::DataStoreInterface")

//==============================================================================
// End of file
//==============================================================================
