//==============================================================================
// CellML file model
//==============================================================================

#include "cellmlfilemodel.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

CellmlFileModel::CellmlFileModel(CellmlFile *pCellmlFile,
                                 iface::cellml_api::Model *pCellmlApiModel) :
    CellmlFileNamedElement(pCellmlFile, pCellmlApiModel)
{
}

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================
// End of file
//==============================================================================
