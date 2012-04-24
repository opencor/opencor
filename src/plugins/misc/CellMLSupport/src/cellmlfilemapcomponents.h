//==============================================================================
// CellML file map components
//==============================================================================

#ifndef CELLMLFILEMAPCOMPONENTS_H
#define CELLMLFILEMAPCOMPONENTS_H

//==============================================================================

#include "cellmlfileelement.h"
#include "cellmlsupportglobal.h"

//==============================================================================

namespace OpenCOR {
namespace CellMLSupport {

//==============================================================================

class CELLMLSUPPORT_EXPORT CellmlFileMapComponents : public CellmlFileElement
{
public:
    explicit CellmlFileMapComponents(iface::cellml_api::MapComponents *pMapComponents);
};

//==============================================================================

}   // namespace CellMLSupport
}   // namespace OpenCOR

//==============================================================================

#endif

//==============================================================================
// End of file
//==============================================================================
