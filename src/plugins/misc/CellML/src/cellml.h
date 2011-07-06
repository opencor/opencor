#ifndef CELLML_H
#define CELLML_H

#include <QString>

namespace OpenCOR {
namespace CellML {

static const QString CellmlHomepageUrl   = "http://www.cellml.org/";
static const QString CellmlFileExtension = "cellml";

enum CellmlVersion
{
    Cellml_1_0,
    Cellml_1_1
};

QString cellmlVersionString(const CellmlVersion &pCellmlVersion);

bool newCellmlFile(const QString &pFileName, const QString &pModelName,
                   const CellmlVersion &pCellmlVersion);

} }

#endif
