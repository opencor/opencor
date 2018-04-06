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

#include <QObject>
#ifndef CLI_VERSION
    #include <QIcon>
#endif

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreArray
{
public:
    explicit DataStoreArray(const quint64 &pSize);
    ~DataStoreArray();

    quint64 size() const;

    double * data() const;
    double data(const quint64 &pPosition) const;

    void clear();

    void decRef();
    void incRef();
    int refCount() const;

private:
    const quint64 mSize;
    int mRefCount;
    double *mData;
};

//==============================================================================

class DataStoreVariableRun
{
public:
    explicit DataStoreVariableRun(const quint64 &pCapacity, double *pValue);
    ~DataStoreVariableRun();

    quint64 size() const;

    void addValue();
    void addValue(const double &pValue);

    DataStoreArray * array() const;
    double value(const quint64 &pPosition) const;
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
    explicit DataStoreVariable(double *pValue = 0);
    ~DataStoreVariable();

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    void addRun(const quint64 &pCapacity);
    void keepRuns(const int &pRunsCount);

    int runsCount() const;

    void addRun(const quint64 &pCapacity);
    void keepRuns(const int &pRunsCount);

#ifndef CLI_VERSION
    QIcon icon() const;
    void setIcon(const QIcon &pIcon);
#endif

    void setUri(const QString &pUri);

    void setLabel(const QString &pLabel);

    void setUnit(const QString &pUnit);

    void addValue();
    void addValue(const double &pValue);

    double getValue() const;
    void setValue(const double &pValue);

    DataStoreArray * array(const int &pRun = -1) const;

public slots:
    bool isVisible() const;

    int runsCount() const;

    QString uri() const;

    QString label() const;

    QString unit() const;

    quint64 size(const int &pRun = -1) const;

    double value(const quint64 &pPosition, const int &pRun = -1) const;
    double * values(const int &pRun = -1) const;

private:
#ifndef CLI_VERSION
    QIcon mIcon;
#endif
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

class DataStoreData
{
public:
    explicit DataStoreData(const QString &pFileName, DataStore *pDataStore,
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
    explicit DataStore(const QString &pUri);
    ~DataStore();

    bool addRun(const quint64 &pCapacity);

    DataStoreVariables variables();
    DataStoreVariables voiAndVariables();

    DataStoreVariable * addVariable(double *pValue = 0);
    DataStoreVariables addVariables(double *pValues, const int &pCount);

    void addValues(const double &pVoiValue);

public slots:
    QString uri() const;

    int runsCount() const;

    quint64 size(const int &pRun = -1) const;

    OpenCOR::DataStore::DataStoreVariable * voi() const;

private:
    QString mUri;

    DataStoreVariable *mVoi;
    DataStoreVariables mVariables;
};

//==============================================================================

class DataStoreExporter : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreExporter(DataStoreData *pDataStoreData);
    ~DataStoreExporter();

    void start();

    virtual void execute(QString &pErrorMessage) const = 0;

private:
    QThread *mThread;

protected:
    DataStoreData *mDataStoreData;

signals:
    void done(const QString &pErrorMessage);
    void progress(const double &pProgress) const;

private slots:
    void started();
};

//==============================================================================

}   // namespace DataStore

//==============================================================================

extern "C" Q_DECL_EXPORT int dataStoreInterfaceVersion();

//==============================================================================

class DataStoreInterface
{
public:
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
