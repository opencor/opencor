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
#include "bgitemattribute.h"

namespace OpenCOR {
namespace BondGraphUI {
// attributes

BGItemAttribute::BGItemAttribute() : flags(ATTR_NONE) {
  valueType = QVariant::String;
}

BGItemAttribute::BGItemAttribute(const QByteArray &attrId,
                                 const QString &attrName)
    : flags(ATTR_NODEFAULT) {
  id = attrId;
  name = attrName;
  if (name.isEmpty())
    name = id;

  valueType = QVariant::String;
}

BGItemAttribute::BGItemAttribute(const QByteArray &attrId,
                                 const QString &attrName,
                                 const QVariant &defaultValue_,
                                 const int attrFlags_)
    : flags(attrFlags_) {
  id = attrId;
  name = attrName.isEmpty() ? id : attrName;

  valueType = defaultValue_.type();
  defaultValue = (flags & ATTR_NODEFAULT) ? QVariant() : defaultValue_;
}

bool BGItemAttribute::storeTo(QDataStream &out, quint64 /*version64*/) const {
  out << id << name << defaultValue << true << true << valueType;

  return true;
}

bool BGItemAttribute::restoreFrom(QDataStream &out, quint64 version64) {
  Q_UNUSED(version64);
  out >> id;

  static bool dummy;

  out >> name >> defaultValue >> dummy >> dummy;
  // attrFlags = ATTR_USER;

  out >> valueType;

  return true;
}

// attribute constrains

BGItemAttributeConstraints::~BGItemAttributeConstraints() {
  // dummy
}

} // namespace BondGraphUI
} // namespace OpenCOR