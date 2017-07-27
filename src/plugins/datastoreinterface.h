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
    explicit DataStoreArray(const qulonglong &pCapacity);

    qulonglong capacity() const;
    void decReference();
    void incReference();
    double * values() const;

private:
    ~DataStoreArray();

    const qulonglong mCapacity;
    int mReferences;
    double *mValues;
};

//==============================================================================

class DataStoreVariable : public QObject
{
    Q_OBJECT

public:
    explicit DataStoreVariable(const qulonglong &pCapacity = 0, double *pNextValuePtr = 0);
    ~DataStoreVariable();

    void createArray(const qulonglong &pCapacity);
    void deleteArray();

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    bool isVisible() const;

#ifndef CLI_VERSION
    QIcon icon() const;
    void setIcon(const QIcon &pIcon);
#endif

    void setUri(const QString &pUri);

    void setLabel(const QString &pLabel);

    void setUnit(const QString &pUnit);

    void clearNextValuePtr();
    void setNextValuePtr(double *pNextValuePtr);

    void addValue();
    void addValue(const double &pValue);

    double * values() const;

    DataStoreArray * array() const;

public slots:
    QString uri() const;

    QString label() const;

    QString unit() const;

    qulonglong size() const;

    double nextValue() const;
    void setNextValue(const double &pValue);

    double value(const qulonglong &pPosition) const;

private:
#ifndef CLI_VERSION
    QIcon mIcon;
#endif
    QString mUri;
    QString mName;
    QString mUnit;

    qulonglong mCapacity;
    qulonglong mSize;

    DataStoreArray *mArray;
    double *mNextValuePtr;
    double *mValues;
};

//==============================================================================

//typedef QList<DataStoreVariable *> DataStoreVariables;


class DataStoreVariables : public QList<DataStoreVariable *>
{
public:
    void clearNextValuePtrs();
    void setNextValuePtrs(double *pNextValuePtrs);
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

class DataStore : public QObject
{
    Q_OBJECT

public:
    explicit DataStore(const QString &pUri);
    ~DataStore();

    void createArrays(const qulonglong &pCapacity);
    void deleteArrays();

    DataStoreVariables voiAndVariables();

    DataStoreVariable * addVoi();

    DataStoreVariables variables();
    DataStoreVariable * addVariable(double *pNextValuePtr = 0);
    DataStoreVariables addVariables(const int &pCount, double *pNextValuePtrs);

    void addValues(const double &pVoiValue);

public slots:
    QString uri() const;

    qulonglong capacity() const;
    qulonglong size() const;

    OpenCOR::DataStore::DataStoreVariable * voi() const;

private:
    QString mlUri;

    qulonglong mCapacity;
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
