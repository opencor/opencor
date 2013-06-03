//==============================================================================
// CellML file CellML exporter
//==============================================================================

#include "cellmlfilecellmlexporter.h"

//==============================================================================

//#include "cellml-api-cxx-support.hpp"

#include "CellMLBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellmlExporter::CellmlFileCellmlExporter(iface::cellml_api::Model *pModel,
                                                   const std::wstring &pVersion) :
    CellmlFileExporter(),
    mModel(pModel)
{
    // Create an empty CellML model of the required version

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();

    mExportedModel = cellmlBootstrap->createModel(pVersion);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
