//==============================================================================
// CellML file model
//==============================================================================

#ifndef CELLMLFILEMODEL_H
#define CELLMLFILEMODEL_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileModel : public CellmlFileNamedElement
{
public:
    explicit CellmlFileModel(iface::cellml_api::Model *pModel);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
