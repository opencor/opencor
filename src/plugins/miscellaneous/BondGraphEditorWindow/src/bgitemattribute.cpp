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
namespace BondGraphEditorWindow {
// attributes

BGItemAttribute::BGItemAttribute() :
    flags(ATTR_NONE)
{
    valueType = QVariant::String;
}

bool BGItemAttribute::isUserDefined() const
{
    return !(flags & ATTR_FIXED);
}

BGItemAttribute::BGItemAttribute(const QByteArray &attr_id,
                                 const QString &attr_name) :
    flags(ATTR_NODEFAULT)
{
    id = attr_id;
    name = attr_name;
    if (name.isEmpty())
        name = id;

    valueType = QVariant::String;
}

BGItemAttribute::BGItemAttribute(const QByteArray &attr_id,
                                 const QString &attr_name,
                                 const QVariant &default_value,
                                 const int attr_flags) :
    flags(attr_flags)
{
    id = attr_id;
    name = attr_name.isEmpty() ? id : attr_name;

    valueType = default_value.type();
    defaultValue = (flags & ATTR_NODEFAULT) ? QVariant() : default_value;
}

bool BGItemAttribute::storeTo(QDataStream &out, quint64 /*version64*/) const
{
    out << id << name << defaultValue << true << true << valueType;

    return true;
}

bool BGItemAttribute::restoreFrom(QDataStream &out, quint64 version64)
{
    Q_UNUSED(version64);
    out >> id;

    static bool dummy;

    out >> name >> defaultValue >> dummy >> dummy;
    // attrFlags = ATTR_USER;

    out >> valueType;

    return true;
}

// attribute constrains

BGItemAttributeConstraints::~BGItemAttributeConstraints()
{
    // dummy
}

BGItemAttributeIntegerConstraints::BGItemAttributeIntegerConstraints(int min_v, int max_v) :
    minValue(min_v), maxValue(max_v)
{
}

BGItemAttributeIntegerConstraints::BGItemAttributeIntegerConstraints(BGItemAttributeConstraints *ptr)
{
    auto *dptr =
        dynamic_cast<BGItemAttributeIntegerConstraints *>(ptr);
    if (dptr != nullptr) {
        minValue = dptr->minValue;
        maxValue = dptr->maxValue;
    }
}

BGItemAttributeRationalConstraints::BGItemAttributeRationalConstraints(double min_v, double max_v, int decs) :
    minValue(min_v), maxValue(max_v), decPoints(decs)
{
}

BGItemAttributeRationalConstraints::BGItemAttributeRationalConstraints(
    BGItemAttributeConstraints *ptr)
{
    auto *dptr =
        dynamic_cast<BGItemAttributeRationalConstraints *>(ptr);
    if (dptr != nullptr) {
        minValue = dptr->minValue;
        maxValue = dptr->maxValue;
        decPoints = dptr->decPoints;
    }
}

QMap<int, QIcon> BGAttributeConstraintsListBase::iconsAsMap() const
{
    QMap<int, QIcon> result;

    for (auto i = 0; i < icons.size(); ++i) {
        result[i] = icons[i];
    }

    return result;
}

} // namespace BondGraphEditorWindow
} // namespace OpenCOR