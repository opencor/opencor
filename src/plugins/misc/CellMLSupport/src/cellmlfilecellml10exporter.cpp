//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include "AnnoToolsBootstrap.hpp"
#include "CellMLBootstrap.hpp"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileCellml10Exporter::CellmlFileCellml10Exporter(iface::cellml_api::Model *pModel,
                                                       const QString &pFileName) :
    CellmlFileExporter()
{
    // Create an empty CellML 1.0 model

    ObjRef<iface::cellml_api::CellMLBootstrap> cellmlBootstrap = CreateCellMLBootstrap();
    ObjRef<iface::cellml_api::Model> exportedModel = cellmlBootstrap->createModel(L"1.0");

    // Set the model's name and id

    exportedModel->name(pModel->name());

    std::wstring cmetaId = pModel->cmetaId();

    if (cmetaId.length())
        exportedModel->cmetaId(cmetaId);

    // Create an annotation set to manage annotations

    ObjRef<iface::cellml_services::AnnotationToolService> annotationToolService = CreateAnnotationToolService();

    mAnnotationSet = annotationToolService->createAnnotationSet();

    // Save the exported model

    mResult = saveModel(exportedModel, pFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
