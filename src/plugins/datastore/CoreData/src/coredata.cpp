/*******************************************************************************

Licensed to the OpenCOR team under one or more contributor license agreements.
See the NOTICE.txt file distributed with this work for additional information
regarding copyright ownership. The OpenCOR team licenses this file to you under
the Apache License, Version 2.0 (the "License"); you may not use this file
except in compliance with the License. You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.

*******************************************************************************/

//==============================================================================
// A store for simulation data
//==============================================================================

#include "coredata.h"

//==============================================================================

namespace OpenCOR {
namespace CoreData {

//==============================================================================

DataVariable::DataVariable(const qulonglong &pSize,
                           const double *pValuePointer) :
    mUri(QString()),
    mUnits(QString()),
    mLabel(QString()),
    mValuePointer(pValuePointer),
    mSize(pSize)
{
    mBuffer = new double[pSize];
}

//==============================================================================

DataVariable::~DataVariable()
{
    delete[] mBuffer;
}

//==============================================================================

void DataVariable::savePoint(const qulonglong &pPos)
{
    Q_ASSERT(pPos < mSize);

    if (mValuePointer)
        mBuffer[pPos] = *mValuePointer;
}

//==============================================================================

void DataVariable::savePoint(const qulonglong &pPos, const double &pValue)
{
    Q_ASSERT(pPos < mSize);

    mBuffer[pPos] = pValue;
}

//==============================================================================

double DataVariable::getPoint(const qulonglong &pPos) const
{
    Q_ASSERT(pPos < mSize);

    return mBuffer[pPos];
}

//==============================================================================

const double *DataVariable::getData() const
{
    return mBuffer;
}

//==============================================================================

qulonglong DataVariable::getSize() const
{
    return mSize;
}

//==============================================================================

void DataVariable::setUri(const QString &pUri)
{
    mUri = pUri;
}

//==============================================================================

void DataVariable::setUnits(const QString &pUnits)
{
    mUnits = pUnits;
}

//==============================================================================

void DataVariable::setLabel(const QString &pLabel)
{
    mLabel = pLabel;
}

//==============================================================================

QString DataVariable::getUri() const
{
    return mUri;
}

//==============================================================================

QString DataVariable::getUnits() const
{
    return mUnits;
}

//==============================================================================

QString DataVariable::getLabel() const
{
    return mLabel;
}

//==============================================================================

DataSet::DataSet(const qulonglong &pSize) :
    mSize(pSize),
    mVariables(0),
    mVoi(0)
{
}

//==============================================================================

DataSet::~DataSet()
{
    if (mVoi)
        delete mVoi;

    for (QVector<DataVariable *>::Iterator iter = mVariables.begin(),
                                           iterEnd = mVariables.end();
         iter != iterEnd; ++iter) {
        delete *iter;
    }
}

//==============================================================================

DataVariable * DataSet::getVoi() const
{
    return mVoi;
}

//==============================================================================

DataVariable * DataSet::getVariable(long index) const
{
    return mVariables[index];
}

//==============================================================================

const QVector<DataVariable *> &DataSet::getVariables() const
{
    return mVariables;
}

//==============================================================================

DataVariable * DataSet::holdPoint(const double *pPoint, const bool &pVoi)
{
    if (pVoi && mVoi)
        delete mVoi;

    DataVariable *var = new DataVariable(mSize, pPoint);

    if (pVoi)
        mVoi = var;
    else
        mVariables.push_back(var);

    return var;
}

//==============================================================================

QVector<DataVariable *> DataSet::holdPoints(const long &pCount,
                                            const double *pPoints)
{
    const double *points = pPoints;

    QVector<DataVariable *> vars(pCount);

    for (long i = 0;  i < pCount;  ++i, ++points) {
        DataVariable *variable = new DataVariable(mSize, points);

        mVariables.push_back(variable);

        vars[i] = variable;
    }

    return vars;
}

//==============================================================================

void DataSet::savePoints(const qulonglong &pPos)
{
    for (QVector<DataVariable *>::Iterator iter = mVariables.begin(),
                                           iterEnd = mVariables.end();
         iter != iterEnd; ++iter) {
        (*iter)->savePoint(pPos);
    }
}

//==============================================================================

qulonglong DataSet::getSize() const
{
    return mSize;
}

//==============================================================================

long DataSet::length() const
{
    return mVariables.size();
}

//==============================================================================

}   // namespace CoreData
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
