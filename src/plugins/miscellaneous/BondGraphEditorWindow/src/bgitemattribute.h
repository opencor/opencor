/*******************************************************************************

Copyright (C) The University of Auckland

OpenCOR is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OpenCOR is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://gnu.org/licenses>.

*******************************************************************************/
#pragma once

#include <QByteArray>
#include <QDataStream>
#include <QIcon>
#include <QList>
#include <QMap>
#include <QString>
#include <QVariant>

#include <graphbase.h>

namespace OpenCOR {
namespace BondGraphEditorWindow {
// attribute class

struct BGItemAttribute : public AttrInfo {
    BGItemAttribute();
    BGItemAttribute(const QByteArray &attrId,
                    const QString &attrName = QString());
    BGItemAttribute(const QByteArray &attrId, const QString &attrName,
                    const QVariant &defaultValue,
                    const int attrFlags = ATTR_NONE);
    ~BGItemAttribute();
    int flags = ATTR_NONE;

    bool isUserDefined() const;
    // serialization
    virtual bool storeTo(QDataStream &out, quint64 version64) const;
    virtual bool restoreFrom(QDataStream &out, quint64 version64);

    // dummy member
    QByteArray classId;
};

using AttributesMap = QMap<QByteArray, BGItemAttribute>; // <attr.id, attr>

using ClassAttributesMap = QMap<QByteArray, AttributesMap>; // <class.id, attr.map>

// attribute constraints

using ClassAttrIndex = QPair<QByteArray, QByteArray>;

struct BGItemAttributeConstraints;
using AttributeConstrainsMap = QMap<ClassAttrIndex, BGItemAttributeConstraints *>;

struct BGItemAttributeConstraints {
    virtual ~BGItemAttributeConstraints();
};

// integer values

struct BGItemAttributeIntegerConstraints : public BGItemAttributeConstraints {
    BGItemAttributeIntegerConstraints(int minV = INT_MIN, int maxV = INT_MAX);
    BGItemAttributeIntegerConstraints(BGItemAttributeConstraints *ptr = nullptr);
    int minValue = INT_MIN;
    int maxValue = INT_MAX;
};

// double values

struct BGItemAttributeRationalConstraints : public BGItemAttributeConstraints {
    BGItemAttributeRationalConstraints(double minV, double maxV, int decs = 4);
    BGItemAttributeRationalConstraints(
        BGItemAttributeConstraints *ptr = nullptr);

    double minValue = std::numeric_limits<double>::lowest();
    double maxValue = std::numeric_limits<double>::max();
    int decPoints = 4;
};

// list of id items

using IconsList = QList<QIcon>;

struct BGAttributeConstraintsListBase : public BGItemAttributeConstraints {
    QStringList names;
    IconsList icons;
    // convenience method to conform property browser API
    QMap<int, QIcon> iconsAsMap() const;
};

struct BGAttributeConstraintsList : public BGAttributeConstraintsListBase {
    QStringList ids;
};

struct BGAttributeConstraintsEnum : public BGAttributeConstraintsListBase {
    QList<int> ids;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
