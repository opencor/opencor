//==============================================================================
// CellML file CellML 1.1 exporter
//==============================================================================

#include "cellmlfilecellml11exporter.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellml11Exporter::CellmlFileCellml11Exporter(iface::cellml_api::Model *pModel,
                                                       const QString &pFileName) :
    CellmlFileCellmlExporter(pModel, L"1.1")
{
    // Save our exported model

    mResult = saveModel(mExportedModel, pFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
