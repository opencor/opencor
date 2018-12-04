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

class DataStoreArray
{
public:
    explicit DataStoreArray(quint64 pSize);

    quint64 size() const;

    double * data() const;
    double data(quint64 pPosition) const;

    void clear();

    void decRef();
    void incRef();

private:
    const quint64 mSize;
    int mRefCount;
    double *mData;

    ~DataStoreArray();
};

//==============================================================================

class DataStoreValue : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreValue(double *pValue = 0);
    ~DataStoreValue();

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
    explicit DataStoreValues();
    explicit DataStoreValues(DataStoreArray *pDataStoreArray);
    ~DataStoreValues();
};

//==============================================================================

class DataStoreVariableRun : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariableRun(quint64 pCapacity, double *pValue);
    ~DataStoreVariableRun();

    quint64 size() const;

    DataStoreArray * array() const;

    void addValue();
    void addValue(double pValue);

    double valueAt(quint64 pPosition) const;
    double * values() const;

private:
    quint64 mCapacity;
    quint64 mSize;

    DataStoreArray *mArray;
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

    void addRun(quint64 pCapacity);
    void keepRuns(int pRunsCount);

    int type() const;
    void setType(int pType);

    void setUri(const QString &pUri);

    void setLabel(const QString &pLabel);

    void setUnit(const QString &pUnit);

    DataStoreArray * array(int pRun = -1) const;

    void addValue();
    void addValue(double pValue);

    double * values(int pRun = -1) const;

public slots:
    bool isVisible() const;

    int runsCount() const;

    QString uri() const;

    QString label() const;

    QString unit() const;

    quint64 size(int pRun = -1) const;

    double getValue() const;
    void setValue(double pValue);

    double valueAt(quint64 pPosition, int pRun = -1) const;

private:
    int mType;
    QString mUri;
    QString mName;
    QString mUnit;

    double *mValue;

    DataStoreVariableRuns mRuns;
};

//==============================================================================

class DataStoreVariables : public QList<DataStoreVariable *>
{
};

//==============================================================================

class DataStore;

//==============================================================================

class DataStore;

//==============================================================================

class DataStoreExportData : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreExportData(const QString &pFileName,
                                 DataStore *pDataStore,
                                 const DataStoreVariables &pVariables);

    QString fileName() const;
    DataStore * dataStore() const;
    DataStoreVariables variables() const;

private:
    QString mFileName;
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

    bool addRun(quint64 pCapacity);

    DataStoreVariables variables();
    DataStoreVariables voiAndVariables();

    DataStoreVariables addVariables(double *pValues, int pCount);

    void addValues(double pVoiValue);

public slots:
    QString uri() const;

    int runsCount() const;

    quint64 size(int pRun = -1) const;

    OpenCOR::DataStore::DataStoreVariable * voi() const;

private:
    QString mUri;

    DataStoreVariable *mVoi;
    DataStoreVariables mVariables;
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
