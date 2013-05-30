//==============================================================================
// CellML file exporter
//==============================================================================

#include "cellmlfileexporter.h"

//==============================================================================

#include <QFile>
#include <QTextStream>

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileExporter::CellmlFileExporter() :
    mResult(false)
{
}

//==============================================================================

bool CellmlFileExporter::result() const
{
    // Return the result of the conversion

    return mResult;
}

//==============================================================================

bool CellmlFileExporter::saveModel(iface::cellml_api::Model *pModel,
                                   const QString &pFileName)
{
    // Save the given model

    QFile file(pFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.remove();

        return false;
    } else {
        QTextStream out(&file);

        out << QString::fromStdWString(pModel->serialisedText());

        file.close();

        return true;
    }
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
