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

//==============================================================================
// Mesh file reader
//==============================================================================

#include "mappingviewmeshreader.h"

//==============================================================================

#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace MappingView {

//==============================================================================

MappingViewMeshReader::MappingViewMeshReader(QString pName):
    pFileName(pName)
{
}

//==============================================================================

QStringList MappingViewMeshReader::getNodesNames()
{
    QStringList list = QStringList();

    QFile file(pFileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return list;
    }
    QTextStream in(&file);
    QString line;
    QStringList words;

    while (!in.atEnd()) {
        line = in.readLine();
        words = line.split(' ');
        words.removeAll("");
        if (words.first()==QString("Node:")) {
             list << words[1];
        }
    }

    list.sort(Qt::CaseSensitive);
    return list;
}

//==============================================================================

} // namespace MappingView
} // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
