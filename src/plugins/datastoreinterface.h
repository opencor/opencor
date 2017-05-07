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

class DataStoreVariable
{
public:
    explicit DataStoreVariable(const qulonglong &pCapacity, double *pValue = 0);
    ~DataStoreVariable();

    static bool compare(DataStoreVariable *pVariable1,
                        DataStoreVariable *pVariable2);

    bool isVisible() const;

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

    qulonglong size() const;

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

    const qulonglong mCapacity;
    qulonglong mSize;

    double *mValue;
    double *mValues;
};

//==============================================================================

typedef QList<DataStoreVariable *> DataStoreVariables;

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
