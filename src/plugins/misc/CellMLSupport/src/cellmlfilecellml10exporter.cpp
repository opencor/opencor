//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include "cellml-api-cxx-support.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellml10Exporter::CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                                       const QString &pFileName) :
    CellmlFileExporter()
{
    ObjRef<iface::cellml_api::Model> exportedModel = pModel;

    // Save the exported model

    mResult = saveModel(exportedModel, pFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
