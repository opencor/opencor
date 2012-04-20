//==============================================================================
// CellML file component
//==============================================================================

#ifndef CELLMLFILECOMPONENT_H
#define CELLMLFILECOMPONENT_H

//==============================================================================

#include "cellmlfilenamedelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileComponent : public CellmlFileNamedElement
{
public:
    explicit CellmlFileComponent(iface::cellml_api::ImportComponent *pImportComponent);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
