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

#include <QVector>

//==============================================================================

namespace OpenCOR {
namespace DataStore {

//==============================================================================

class DataStoreVariable
{
public:
    explicit DataStoreVariable(const qulonglong &pSize, double *pValue = 0);
    virtual ~DataStoreVariable();

    bool isValid() const;

    QString uri() const;
    void setUri(const QString &pUri);

    QString label() const;
    void setLabel(const QString &pLabel);

    QString unit() const;
    void setUnit(const QString &pUnit);

    qulonglong size() const;

    void setValue(const qulonglong &pPosition);
    void setValue(const qulonglong &pPosition, const double &pValue);

    double value(const qulonglong &pPosition) const;
    double * values() const;

private:
    QString mUri;
    QString mName;
    QString mUnit;

    qulonglong mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

typedef QVector<DataStoreVariable *> DataStoreVariables;

//==============================================================================

class DataStore
{
public:
    explicit DataStore(const QString &pId, const QString &pUri,
                       const qulonglong &pSize);
    virtual ~DataStore();

    QString id() const;
    QString uri() const;

    qulonglong size() const;

    DataStoreVariable * voi() const;
    DataStoreVariable * addVoi();

    DataStoreVariables variables();
    DataStoreVariable * addVariable(double *pValue = 0);
    DataStoreVariables addVariables(const int &pCount, double *pValues);

    void setValues(const qulonglong &pPosition, const double &pValue);

private:
    QString mId;
    QString mlUri;

    const qulonglong mSize;

    DataStoreVariable *mVoi;
    DataStoreVariables mVariables;
};

//==============================================================================

class DataStoreExporter
{
public:
    explicit DataStoreExporter(const QString &pId = QString());
    virtual ~DataStoreExporter();

    virtual void execute(const QString &pFileName,
                         DataStore *pDataStore) const = 0;

    QString id() const;

private:
    QString mId;
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
