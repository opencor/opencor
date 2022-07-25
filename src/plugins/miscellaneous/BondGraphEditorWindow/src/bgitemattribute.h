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

  /*const*/ int flags = ATTR_NONE;

  bool isUserDefined() const { return !(flags & ATTR_FIXED); }

  // serialization
  virtual bool storeTo(QDataStream &out, quint64 version64) const;
  virtual bool restoreFrom(QDataStream &out, quint64 version64);

  // dummy member
  QByteArray classId;
};

typedef QMap<QByteArray, BGItemAttribute> AttributesMap; // <attr.id, attr>

typedef QMap<QByteArray, AttributesMap>
    ClassAttributesMap; // <class.id, attr.map>

// attribute constrains

typedef QPair<QByteArray, QByteArray> ClassAttrIndex;

struct BGItemAttributeConstraints;
typedef QMap<ClassAttrIndex, BGItemAttributeConstraints *>
    AttributeConstrainsMap;

struct BGItemAttributeConstraints {
  virtual ~BGItemAttributeConstraints();
};

// integer values

struct BGItemAttributeIntegerConstraints : public BGItemAttributeConstraints {
  BGItemAttributeIntegerConstraints(int minV = INT_MIN, int maxV = INT_MAX) {
    minValue = minV;
    maxValue = maxV;
  }

  BGItemAttributeIntegerConstraints(BGItemAttributeConstraints *ptr = nullptr) {
    BGItemAttributeIntegerConstraints *dptr =
        dynamic_cast<BGItemAttributeIntegerConstraints *>(ptr);
    if (dptr) {
      minValue = dptr->minValue;
      maxValue = dptr->maxValue;
    }
  }

  int minValue = INT_MIN;
  int maxValue = INT_MAX;
};

// double values

struct BGItemAttributeRationalConstraints : public BGItemAttributeConstraints {
  BGItemAttributeRationalConstraints(double minV, double maxV, int decs = 4) {
    minValue = minV;
    maxValue = maxV;
    decPoints = decs;
  }

  BGItemAttributeRationalConstraints(
      BGItemAttributeConstraints *ptr = nullptr) {
    BGItemAttributeRationalConstraints *dptr =
        dynamic_cast<BGItemAttributeRationalConstraints *>(ptr);
    if (dptr) {
      minValue = dptr->minValue;
      maxValue = dptr->maxValue;
      decPoints = dptr->decPoints;
    }
  }

  double minValue = std::numeric_limits<double>::lowest();
  double maxValue = std::numeric_limits<double>::max();
  int decPoints = 4;
};

// list of id items

typedef QList<QIcon> IconsList;

struct BGAttributeConstraintsListBase : public BGItemAttributeConstraints {
  QStringList names;
  IconsList icons;

  // convenience method to conform property browser API
  QMap<int, QIcon> iconsAsMap() const {
    QMap<int, QIcon> result;

    for (auto i = 0; i < icons.size(); ++i)
      result[i] = icons[i];

    return result;
  }
};

struct BGAttributeConstraintsList : public BGAttributeConstraintsListBase {
  QStringList ids;
};

struct BGAttributeConstraintsEnum : public BGAttributeConstraintsListBase {
  QList<int> ids;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR