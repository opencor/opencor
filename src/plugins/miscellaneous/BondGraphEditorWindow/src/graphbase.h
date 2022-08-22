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
#include <QColor>
#include <QList>
#include <QMap>
#include <QPointF>
#include <QVariant>

namespace OpenCOR {
namespace BondGraphEditorWindow {

typedef QMap<QByteArray, QVariant> GraphAttributes;

enum AttrFlags {
    ATTR_NONE = 0,
    ATTR_VIRTUAL = 1,   // read only, not to be stored & read
    ATTR_FIXED = 2,     // non-user defined
    ATTR_NODEFAULT = 4, // has no default value
    ATTR_MAPPED =
        8 // internal: mapped to some system value, i.e. coordinate or size
};

struct AttrInfo {
    QByteArray id;
    QString name;
    int valueType = 0;
    QVariant defaultValue;
};

typedef QMap<QByteArray, AttrInfo> AttributeInfos;

struct NodePort {
    QString name;
    float x = 0, y = 0;
    QColor color;
    int anchor = 0;
};

typedef QMap<QString, NodePort> NodePorts;

struct Node {
    // generic data
    QByteArray id;
    GraphAttributes attrs;

    // only for ports
    NodePorts ports;
};

struct Edge {
    // generic data
    QByteArray id;
    GraphAttributes attrs;

    QByteArray startNodeId;
    QByteArray endNodeId;

    // only for ports
    QByteArray startPortId;
    QByteArray endPortId;
};

} // namespace BondGraphEditorWindow
} // namespace OpenCOR
