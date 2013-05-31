//==============================================================================
// CellML file CellML 1.0 exporter
//==============================================================================

#include "cellmlfilecellml10exporter.h"

//==============================================================================

#include <QApplication>

//==============================================================================

#include "IfaceCeVAS.hxx"

#include "AnnoToolsBootstrap.hpp"
#include "CellMLBootstrap.hpp"
#include "CeVASBootstrap.hpp"

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

    // Create a CellML Variable Association Service object so that we can find
    // relevant components

    ObjRef<iface::cellml_services::CeVASBootstrap> cevasBootstrap = CreateCeVASBootstrap();
    ObjRef<iface::cellml_services::CeVAS> cevas = cevasBootstrap->createCeVASForModel(pModel);

    std::wstring cevasError = cevas->modelError();

    if (cevasError.length()) {
        mErrorMessage = QObject::tr("a CeVAS object could not be created");

        return;
    }

    // Save the exported model

    mResult = saveModel(exportedModel, pFileName);
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
