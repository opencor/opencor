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

#ifndef CLI_VERSION
    #include <QIcon>
#endif

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreVariableRun
{
public:
    explicit DataStoreVariableRun(const qulonglong &pCapacity, double *pValue);
    ~DataStoreVariableRun();

    qulonglong size() const;

    void addValue();
    void addValue(const double &pValue);

    double value(const qulonglong &pPosition) const;
    double * values() const;

private:
    qulonglong mCapacity;
    qulonglong mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

typedef QList<DataStoreVariableRun *> DataStoreVariableRuns;

//==============================================================================

class DataStoreVariable
{
public:
    explicit DataStoreVariable(double *pValue = 0);
    ~DataStoreVariable();

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    bool isVisible() const;

    int runsCount() const;

    void addRun(const qulonglong &pCapacity);
    void keepRuns(const int &pRunsCount);

#ifndef CLI_VERSION
    QIcon icon() const;
    void setIcon(const QIcon &pIcon);
#endif

    QString uri() const;
    void setUri(const QString &pUri);

    QString label() const;
    void setLabel(const QString &pLabel);

    QString unit() const;
    void setUnit(const QString &pUnit);

    qulonglong size(const int &pRun) const;

    void addValue();
    void addValue(const double &pValue);

    double value(const qulonglong &pPosition) const;
    double * values() const;

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

typedef QList<DataStoreVariable *> DataStoreVariables;

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

class DataStore
{
public:
    explicit DataStore(const QString &pUri);
    ~DataStore();

    QString uri() const;

    bool addRun(const qulonglong &pCapacity);

    qulonglong size(const int &pRun = -1) const;

    DataStoreVariable * voi() const;
    DataStoreVariables variables();
    DataStoreVariables voiAndVariables();

    DataStoreVariable * addVariable(double *pValue = 0);
    DataStoreVariables addVariables(double *pValues, const int &pCount);

    void addValues(const double &pVoiValue);

private:
    QString mlUri;

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
