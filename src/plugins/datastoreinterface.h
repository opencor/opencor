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

    void addRun(quint64 pCapacity);
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
    void addValue(double pValue);

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

class DataStoreImportedData : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreImportedData(const QString &pFileName,
                                   DataStore *pDataStore);

    QString fileName() const;
    DataStore * dataStore() const;

protected:
    QString mFileName;
    DataStore *mDataStore;
};

//==============================================================================

class DataStoreData : public DataStoreImportedData
{
    Q_OBJECT

public:
    explicit DataStoreData(const QString &pFileName, DataStore *pDataStore,
                           const DataStoreVariables &pVariables);

    DataStoreVariables variables() const;

private:
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
    explicit DataStoreImporterWorker(DataStoreImportedData *pDataStoreImportedData);

protected:
    DataStoreImportedData *mDataStoreImportedData;

signals:
    void progress(double pProgress);
    void done(const QString &pErrorMessage);

public slots:
   virtual void run() = 0;
};

//==============================================================================

class DataStoreImporter : public QObject
{
    Q_OBJECT

public:
    void importData(DataStoreImportedData *pDataStoreImportedData);

protected:
    virtual DataStoreImporterWorker * workerInstance(DataStoreImportedData *pDataStoreImportedData) = 0;

signals:
    void progress(double pProgress);
    void done(const QString &pErrorMessage);
};

//==============================================================================

class DataStoreExporterWorker : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreExporterWorker(DataStoreData *pDataStoreData);

protected:
    DataStoreData *mDataStoreData;

signals:
    void progress(double pProgress);
    void done(const QString &pErrorMessage);

public slots:
   virtual void run() = 0;
};

//==============================================================================

class DataStoreExporter : public QObject
{
    Q_OBJECT

public:
    void exportData(DataStoreData *pDataStoreData);

protected:
    virtual DataStoreExporterWorker * workerInstance(DataStoreData *pDataStoreData) = 0;

signals:
    void progress(double pProgress);
    void done(const QString &pErrorMessage);
};

//==============================================================================

}   // namespace DataStore

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

}   // namespace OpenCOR

//==============================================================================

Q_DECLARE_INTERFACE(OpenCOR::DataStoreInterface, "OpenCOR::DataStoreInterface")

//==============================================================================
// End of file
//==============================================================================
