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

#pragma once

//==============================================================================

#include "plugininfo.h"

//==============================================================================

#include <QIcon>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreVariable
{
public:
    explicit DataStoreVariable(const qulonglong &pCapacity, double *pValue = 0);
    ~DataStoreVariable();

    bool isVisible() const;

    QIcon icon() const;
    void setIcon(const QIcon &pIcon);

    QString uri() const;
    void setUri(const QString &pUri);

    QString label() const;
    void setLabel(const QString &pLabel);

    QString unit() const;
    void setUnit(const QString &pUnit);

    qulonglong size() const;

    void addValue();
    void addValue(const double &pValue);

    double value(const qulonglong &pPosition) const;
    double * values() const;

private:
    QIcon mIcon;
    QString mUri;
    QString mName;
    QString mUnit;

    const qulonglong mCapacity;
    qulonglong mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

class DataStoreVariables : public QList<DataStoreVariable *>
{
public:
    void sort();

private:
    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);
};

//==============================================================================

class DataStoreData
{
public:
    explicit DataStoreData(const QString &pFileName,
                           const DataStoreVariables &pSelectedVariables);

    QString fileName() const;
    DataStoreVariables selectedVariables() const;

private:
    QString mFileName;
    DataStoreVariables mSelectedVariables;
};

//==============================================================================

class DataStore
{
public:
    explicit DataStore(const QString &pUri, const qulonglong &pCapacity);
    ~DataStore();

    QString uri() const;

    qulonglong size() const;

    DataStoreVariables voiAndVariables();

    DataStoreVariable * voi() const;
    DataStoreVariable * addVoi();

    DataStoreVariables variables();
    DataStoreVariable * addVariable(double *pValue = 0);
    DataStoreVariables addVariables(const int &pCount, double *pValues);

    void addValues(const double &pVoiValue);

private:
    QString mlUri;

    const qulonglong mCapacity;
    qulonglong mSize;

    DataStoreVariable *mVoi;
    DataStoreVariables mVariables;
};

//==============================================================================

class DataStoreExporter : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreExporter(const QString &pFileName, DataStore *pDataStore,
                               DataStoreData *pDataStoreData);
    ~DataStoreExporter();

    void start();

    virtual void execute(QString &pErrorMessage) const = 0;

private:
    QThread *mThread;

protected:
    QString mFileName;
    DataStore *mDataStore;
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
